// Fill out your copyright notice in the Description page of Project Settings.


#include "Niagara/CrowdActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Niagara/PingMarker.h"


ACrowdActor::ACrowdActor() {
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded()) SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	
	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraSystem->SetupAttachment(SphereMesh);

	Offset = FVector(-100.f, -90.f, 100.f);
}


void ACrowdActor::BeginPlay() {
	Super::BeginPlay();
}


void ACrowdActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector Destination;
	float Speed = FollowSpeed;
	
	if (TargetActor) {
		Destination = TargetActor->GetActorLocation() + Offset;
	} else if (bShouldMove) {
		Destination = TargetLocation;
		Speed /= 2.f;
	} else {
		return;
	}

	if (!TargetActor && bShouldMove) {
		float Distance = FVector::Dist(CurrentLocation, Destination);
		if (Distance < 10.f) {
			bShouldMove = false;

			if (PingMarker && !PingMarker->IsPendingKillPending()) {
				PingMarker->Destroy();
				PingMarker = nullptr;
			}
		}
	}

	FVector NewLocation = FMath::VInterpTo(CurrentLocation, Destination, DeltaTime, Speed);
	SetActorLocation(NewLocation);

}

void ACrowdActor::MoveTo(const FVector& NewTargetLocation) {
	TargetLocation = NewTargetLocation;
	bShouldMove = true;
}

AActor* ACrowdActor::GetTargetActor() { return TargetActor; }

void ACrowdActor::SetTargetActor(AActor* NewTarget) { TargetActor = NewTarget; }
void ACrowdActor::SetPingMarker(APingMarker* NewPingMarker) { PingMarker = NewPingMarker; }

