// Fill out your copyright notice in the Description page of Project Settings.


#include "Niagara/CrowdActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ACrowdActor::ACrowdActor() {
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	if (SphereMeshAsset.Succeeded()) SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
	
	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraSystem->SetupAttachment(SphereMesh);

	Offset = FVector(-150.f, 80.f, 100.f);
}


void ACrowdActor::BeginPlay() {
	Super::BeginPlay();
}


void ACrowdActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (TargetActor) {
		FVector TargetLocation = TargetActor->GetActorLocation() + Offset;
		FVector CurrentLocation = GetActorLocation();
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, FollowSpeed);
		SetActorLocation(NewLocation);
	}
}

