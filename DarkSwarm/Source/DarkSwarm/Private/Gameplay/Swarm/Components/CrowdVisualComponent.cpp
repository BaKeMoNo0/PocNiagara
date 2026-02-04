
#include "Gameplay/Swarm/Components/CrowdVisualComponent.h"
#include "NiagaraComponent.h"
#include "Gameplay/Swarm/CrowdActor.h"
#include "Gameplay/Swarm/Types/CrowdVisualParams.h"
#include "Gameplay/Swarm/Types/SwarmForm.h"
#include "Gameplay/World/Interactive/DisintegratableActor.h"


void UCrowdVisualComponent::Init(UNiagaraComponent* InNiagara, UStaticMeshComponent* InCollisionMesh, UStaticMeshComponent* InSphere){
	Niagara = InNiagara;
	CollisionMesh = InCollisionMesh;
	Sphere = InSphere;
	
	check(Niagara);
	check(CollisionMesh);
	check(Sphere);
}

void UCrowdVisualComponent::Initialize(const FCrowdVisualParams& MaxVisualParams, float ParticleSpacing) {
	check(Niagara);
	Niagara->SetIntParameter(FName("User.SpawnCount"), MaxVisualParams.ParticleCount);
	Niagara->SetFloatParameter(FName("User.Spacing"), ParticleSpacing);
}



void UCrowdVisualComponent::ApplyVisualState(const FCrowdVisualParams& VisualParams) {
	check(Niagara);

	Niagara->SetIntParameter(TEXT("User.ActiveParticleCount"), VisualParams.ParticleCount);
	Niagara->SetFloatParameter(TEXT("User.MeshUniformScale"), VisualParams.MeshScale);

	CollisionMesh->SetCollisionEnabled(VisualParams.bEnableCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void UCrowdVisualComponent::UpdateBlend(float BlendAlpha) {
	check(Niagara);

	Niagara->SetFloatParameter(TEXT("User.CurrentBlendAlpha"), BlendAlpha);
	Niagara->SetVectorParameter(TEXT("User.SpherePos"), Sphere->GetComponentLocation());
}

void UCrowdVisualComponent::UpdateSlowingDown(float DeltaTime, float& InOutBlendAlpha, float BlendTarget){
	InOutBlendAlpha = FMath::FInterpTo(InOutBlendAlpha, BlendTarget, DeltaTime, 0.25f);

	UpdateBlend(InOutBlendAlpha);
	
	if (InOutBlendAlpha > 0.2f) {
		CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		CollisionMesh->SetCollisionResponseToAllChannels(ECR_Block);
		CollisionMesh->SetCollisionObjectType(ECC_WorldStatic);
	}
}




void UCrowdVisualComponent::ApplyFormVisual(ESwarmForm SwarmForm, int ParticleCount, float ParticleSpacing, float MeshScale) {
	check(Niagara);
	check(CollisionMesh);
	
	Niagara->SetIntParameter(FName("User.FormType"), static_cast<int32>(SwarmForm));

	const float UnitMeshSize = 15.0f;
	FVector NewScale = FVector::OneVector;;

	switch (SwarmForm) {
		case ESwarmForm::Cube: {
				check(CubeMesh)
				const int CountPerAxisCube = FMath::CeilToInt(FMath::Pow(ParticleCount, 1.f / 3.f));
				const float Size = CountPerAxisCube * ParticleSpacing * MeshScale;
				NewScale = FVector(Size / UnitMeshSize);
				CollisionMesh->SetStaticMesh(CubeMesh);
				CollisionMesh->RecreatePhysicsState();
				break;
		}
		case ESwarmForm::Plane: {
				check(PlaneMesh)
				const int CountPerAxisPlane = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(ParticleCount)));
				const float Size = CountPerAxisPlane * ParticleSpacing * MeshScale;
				NewScale = FVector(Size / UnitMeshSize, Size / UnitMeshSize, 1.0f);
				CollisionMesh->SetStaticMesh(PlaneMesh);
				CollisionMesh->RecreatePhysicsState();
				break;
		}
		default: break;
	}
	
	if (InvisibleMaterial) CollisionMesh->SetMaterial(0, InvisibleMaterial);

	CollisionMesh->SetWorldScale3D(NewScale);
}

void UCrowdVisualComponent::EnsureActiveNiagara() {
	if (Niagara && !Niagara->IsActive()) Niagara->Activate(true);
}




void UCrowdVisualComponent::BeginAbsorption(ADisintegratableActor* DisActor) {
	if (!DisActor) return;
	
	UNiagaraComponent* Ng = DisActor->GetNiagaraComp();
	if (!Ng) return;
	
	CurrentAbsorbingActor = DisActor;

	Ng->SetFloatParameter(TEXT("User.AttractionStrength"), 50.f);
	Ng->SetFloatParameter(TEXT("User.NoiseForceDesintegration"), 50.f);

	Ng->OnSystemFinished.Clear();
	Ng->OnSystemFinished.AddDynamic(this, &UCrowdVisualComponent::HandleNiagaraFinished);

	Ng->Activate(true);
}


void UCrowdVisualComponent::UpdateAttractionTarget(UNiagaraComponent* Ng) {
	if (!Ng) return;
	Ng->SetVectorParameter(FName("User.AttractionTarget"), Sphere->GetComponentLocation());
}


void UCrowdVisualComponent::HandleNiagaraFinished(UNiagaraComponent* FinishedComponent) {
	if (ACrowdActor* Crowd = Cast<ACrowdActor>(GetOwner())) {
		Crowd->SetActionParticleCount(100);
		Crowd->GetNiagaraSystem()->SetIntParameter(FName("User.SpawnCount"), Crowd->GetActionVisualParams().ParticleCount);
		Crowd->GetNiagaraSystem()->ReinitializeSystem();
	}

	if (CurrentAbsorbingActor){
		OnAbsorptionFinished.Broadcast(CurrentAbsorbingActor);
		CurrentAbsorbingActor = nullptr;
	}
}
