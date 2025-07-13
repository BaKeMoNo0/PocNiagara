// Fill out your copyright notice in the Description page of Project Settings.


#include "Niagara/CrowdActor.h"
#include "NiagaraComponent.h"
#include "Niagara/Component/CrowdPositionGeneratorComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "PlayerCharacter/Component/PlayerPingComponent.h"


ACrowdActor::ACrowdActor() {
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded()) SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMeshAsset.Succeeded()) CollisionMesh->SetStaticMesh(CubeMeshAsset.Object);
	CollisionMesh->SetupAttachment(SphereMesh);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraSystem->SetupAttachment(SphereMesh);

	PositionGeneratorComp = CreateDefaultSubobject<UCrowdPositionGeneratorComponent>(TEXT("CrowdPositionGenerator"));
	
	Offset = FVector(-100.f, -90.f, 100.f);
}


void ACrowdActor::BeginPlay() {
	Super::BeginPlay();

	NiagaraSystem->SetFloatParameter(FName("User.SpawnCount"), SpawnCount);
}


void ACrowdActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateDestination();
	MoveTowardsDestination(DeltaTime);
	UpdateNiagaraBlending(DeltaTime);
}


void ACrowdActor::UpdateDestination() {
	if (TargetActor) {
		Destination = TargetActor->GetActorLocation() + Offset;
	} else if (bShouldMove) {
		Destination = TargetLocation;
	}
}

void ACrowdActor::MoveTowardsDestination(float DeltaTime) {
	if (!bShouldMove && !TargetActor) return;

	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), Destination, DeltaTime, FollowSpeed);
	SetActorLocation(NewLocation);
}


void ACrowdActor::UpdateNiagaraBlending(float DeltaTime) {
	if (!TargetActor && bShouldMove) {
		float Distance = FVector::Dist(GetActorLocation(), Destination);

		if (Distance <= 5.0f && !bHasReachedDestination) {
			/*if (PositionGeneratorComp && NiagaraSystem) {
				PositionGeneratorComp->GeneratePositions(SpawnCount, Spacing, static_cast<int32>(FormType));
				PositionGeneratorComp->PushToNiagara(NiagaraSystem);
			}*/
			
			if (PingComp && PingComp->IsThisMyActiveMarker(CurrentPingMarkerToDestroy)) PingComp->DestroyPingMarker();
			
			bHasReachedDestination = true;
			if (!bIsSlowingDown) bIsSlowingDown = true;
		}

		if (bIsSlowingDown) {
			BlendAlphaTarget = 0.9;
			CurrentBlendAlpha = FMath::Clamp(FMath::FInterpTo(CurrentBlendAlpha, BlendAlphaTarget, DeltaTime, 0.25f),0.0f, 0.9f);
			NiagaraSystem->SetFloatParameter(FName("User.CubeBlendAlpha"), CurrentBlendAlpha);
			NiagaraSystem->SetVectorParameter(FName("User.SpherePos"), SphereMesh->GetComponentLocation());
			if (CurrentBlendAlpha > 0.2f) {
				CollisionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				CollisionMesh->SetCollisionResponseToAllChannels(ECR_Block);
				CollisionMesh->SetCollisionObjectType(ECC_WorldStatic);
			}
		}
	}
}


void ACrowdActor::MoveTo(const FVector& NewTargetLocation) {
	TargetActor = nullptr;
	TargetLocation = NewTargetLocation;
	bShouldMove = true;
	bHasReachedDestination = false;
}


void ACrowdActor::ReturnToPlayer(APlayerCharacter* Player) {
	TargetActor = Player;
	BlendAlphaTarget = 0.0f;
	CurrentBlendAlpha = 0.0f;
	bShouldMove = false;
	bIsSlowingDown = false;
	NiagaraSystem->SetFloatParameter(FName("User.CubeBlendAlpha"), 0.0f);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void ACrowdActor::SetFormType(EFormType NewFormType){
	if (FormType == NewFormType) return;
	FormType = NewFormType;
	
	NiagaraSystem->SetIntParameter(FName("User.FormType"), static_cast<int32>(NewFormType));

	const float AvgParticleScale = 0.15f;
	const float UnitMeshSize = 10.0f;
	FVector NewScale;

	switch (FormType) {
	case EFormType::Cube: {
			const int CountPerAxisCube = FMath::CeilToInt(FMath::Pow(SpawnCount, 1.0f / 3.0f));
			const float CubeSize = CountPerAxisCube * Spacing * AvgParticleScale;
			NewScale = FVector(CubeSize / UnitMeshSize);
			CollisionMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube")));
			break;
	}
	case EFormType::Plane: {
			const int CountPerAxisPlane = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(SpawnCount)));
			const float PlaneSize = CountPerAxisPlane * Spacing * AvgParticleScale;
			NewScale = FVector(PlaneSize / UnitMeshSize, PlaneSize / UnitMeshSize, 1.0f); // très fin
			CollisionMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane")));
			break;
	}
	default:
		return;
	}

	CollisionMesh->SetWorldScale3D(NewScale);
}



AActor* ACrowdActor::GetTargetActor() { return TargetActor; }
EFormType ACrowdActor::GetFormType() const { return FormType; }

int ACrowdActor::GetSpawnCount() const { return SpawnCount; }

UStaticMeshComponent* ACrowdActor::GetCollisionMesh() const { return SphereMesh; }
UNiagaraComponent* ACrowdActor::GetNiagaraSystem(){ return NiagaraSystem; }

void ACrowdActor::SetPingComp(UPlayerPingComponent* PingCompRef) { PingComp = PingCompRef; }
void ACrowdActor::SetTargetActor(AActor* NewTarget) { TargetActor = NewTarget; }
void ACrowdActor::SetSpawnCount(int NewSpawnCount) { SpawnCount = NewSpawnCount;  }