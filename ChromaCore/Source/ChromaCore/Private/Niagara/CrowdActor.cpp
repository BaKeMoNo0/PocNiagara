// Fill out your copyright notice in the Description page of Project Settings.


#include "Niagara/CrowdActor.h"
#include "NiagaraComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "PlayerCharacter/Component/PlayerPingComponent.h"


ACrowdActor::ACrowdActor() {
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded()) SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMeshAsset.Succeeded()) CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
	CubeMesh->SetupAttachment(SphereMesh);
	CubeMesh->SetWorldScale3D(FVector(1.5f, 1.5f, 1.5f));
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraSystem->SetupAttachment(SphereMesh);

	Offset = FVector(-100.f, -90.f, 100.f);
}


void ACrowdActor::BeginPlay() {
	Super::BeginPlay();
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

		if (Distance <= 2.5f && !bHasReachedDestination) {
			PingComp->DestroyPingMarker();
			bHasReachedDestination = true;
			if (!bIsSlowingDown) bIsSlowingDown = true;
		}

		if (bIsSlowingDown) {
			BlendAlphaTarget = 0.9;
			CurrentBlendAlpha = FMath::Clamp(FMath::FInterpTo(CurrentBlendAlpha, BlendAlphaTarget, DeltaTime, 0.15f),0.0f, 0.9f);
			//UE_LOG(LogTemp, Warning, TEXT("CurrentBlendAlpha : %f"), CurrentBlendAlpha);
			NiagaraSystem->SetFloatParameter(FName("User.CubeBlendAlpha"), CurrentBlendAlpha);
			NiagaraSystem->SetVectorParameter(FName("User.SpherePos"), SphereMesh->GetComponentLocation());
			if (CurrentBlendAlpha > 0.5f) {
				CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				CubeMesh->SetCollisionResponseToAllChannels(ECR_Block);
				CubeMesh->SetCollisionObjectType(ECC_WorldStatic);
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
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


AActor* ACrowdActor::GetTargetActor() { return TargetActor; }
UNiagaraComponent* ACrowdActor::GetNiagaraSystem(){ return NiagaraSystem; }

void ACrowdActor::SetPingComp(UPlayerPingComponent* PingCompRef) { PingComp = PingCompRef; }
void ACrowdActor::SetTargetActor(AActor* NewTarget) { TargetActor = NewTarget; }

void ACrowdActor::SetFormType(EFormType NewFormType){
	FormType = NewFormType;
	NiagaraSystem->SetIntParameter(FName("User.FormType"), static_cast<int32>(NewFormType));
}
