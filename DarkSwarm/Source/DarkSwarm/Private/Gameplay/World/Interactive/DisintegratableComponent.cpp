#include "Gameplay/World/Interactive/DisintegratableComponent.h"
#include "Gameplay/World/Interactive/DesintegrationActor.h"
#include "NiagaraComponent.h"
#include "Core/DarkSwarmGameMode.h"
#include "Gameplay/Swarm/CrowdActor.h"

UDisintegratableComponent::UDisintegratableComponent() {
	PrimaryComponentTick.bCanEverTick = true;

}


void UDisintegratableComponent::BeginPlay() {
	Super::BeginPlay();
	
	GM = Cast<ADarkSwarmGameMode>(GetWorld()->GetAuthGameMode());
	
	Owner = Cast<ADesintegrationActor>(GetOwner());
	if (Owner && Owner->GetMesh()) {
		DynMat = Owner->GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMat) DynMat->SetScalarParameterValue("Opacity", 1.0f);
	}
}


void UDisintegratableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsDisintegrating && Owner->GetNiagaraComp()) {

		if (GM && GM->GetCrowdActor() && GM->GetCrowdActor()->GetCollisionMesh()) {
				CurrentCrowdLocation = GM->GetCrowdActor()->GetCollisionMesh()->GetComponentLocation();
				Owner->GetNiagaraComp()->SetVectorParameter(FName("User.AttractionTarget"), CurrentCrowdLocation);
		}
	}

}

void UDisintegratableComponent::TriggerDisintegration() {
	if (Owner) {
		UNiagaraComponent *Niagara = Owner->GetNiagaraComp();
		if (Owner->GetNiagaraComp()) {
			bIsDisintegrating = true;

			Owner->GetNiagaraComp()->Activate(true);

			if (GM && GM->GetCrowdActor() && GM->GetCrowdActor()->GetCollisionMesh()) {
					CurrentCrowdLocation = GM->GetCrowdActor()->GetCollisionMesh()->GetComponentLocation();
					//DrawDebugSphere(GetWorld(), CurrentCrowdLocation, 20.f, 12, FColor::Green, false, 2.0f);
					Niagara->SetVectorParameter(FName("User.AttractionTarget"), CurrentCrowdLocation);
			}
			Niagara->SetFloatParameter(FName("User.AttractionStrength"), 50.0f);
			Niagara->SetFloatParameter(FName("User.NoiseForceDesintegration"), 50.0f);

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				[this, Niagara]() {
					bIsDisintegrating = false;
					Niagara->Deactivate();
					int SpawnCount = GM->GetCrowdActor()->GetTotalSpawnCount();
					GM->GetCrowdActor()->SetTotalSpawnCount(SpawnCount + 1000);
					GM->GetCrowdActor()->GetNiagaraSystem()->SetIntParameter(FName("User.SpawnCount"), SpawnCount);
					GM->GetCrowdActor()->SetFormType(GM->GetCrowdActor()->GetFormType());
					GM->GetCrowdActor()->GetNiagaraSystem()->ReinitializeSystem();
				},
				5.0f,
				false
			);
		}
		
		FadeOutQuick();
		if (Owner->GetMesh()) Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (Owner->GetSphereCollision()) Owner->GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


void UDisintegratableComponent::FadeOutQuick() {
	if (Owner && Owner->GetMesh()) {
		if (!DynMat) return;

		float Opacity = 1.0f;
		const float OpacityStep = 0.05f;

		TWeakObjectPtr<UMaterialInstanceDynamic> WeakDynMat = DynMat;

		GetWorld()->GetTimerManager().SetTimer(FadeHandle, [this, WeakDynMat, Opacity, OpacityStep]() mutable {
			float NewOpacity = FMath::Clamp(Opacity - OpacityStep, 0.f, 1.f);
			DynMat->SetScalarParameterValue("Opacity", NewOpacity);
			Opacity = NewOpacity;

			if (Opacity <= 0.f) {
				GetWorld()->GetTimerManager().ClearTimer(FadeHandle);
				if (Owner->GetMesh()) {
					Owner->GetMesh()->SetVisibility(false);
					Owner->GetMesh()->DestroyComponent();
				}
			}
		}, 0.1f, true);
	}
}

