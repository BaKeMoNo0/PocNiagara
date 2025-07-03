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

	UE_LOG(LogTemp, Warning, TEXT("no"));
	if (!TargetActor && bShouldMove) {
		float Distance = FVector::Dist(CurrentLocation, Destination);
		UE_LOG(LogTemp, Warning, TEXT("why"));
		
		if (Distance < 10.f && !bIsSlowingDown)  bIsSlowingDown = true;
		
		UE_LOG(LogTemp, Warning, TEXT("here"));
		
		if (PingMarker && !PingMarker->IsPendingKillPending()) {
			PingMarker->Destroy();
			PingMarker = nullptr;
		}

		if (bIsSlowingDown) {
			UE_LOG(LogTemp, Warning, TEXT("isSlowing"));
			CurrentSpeedLimit = FMath::FInterpTo(CurrentSpeedLimit, 0.0f, DeltaTime, 57);
			NiagaraSystem->SetFloatParameter(FName("User.SpeedLimit"), CurrentSpeedLimit);

			if (CurrentSpeedLimit <= 50.f) {
				SetAllStrengthToZero();
				bShouldMove = false;
				bIsSlowingDown = false;
			}
		}
	}

	FVector NewLocation = FMath::VInterpTo(CurrentLocation, Destination, DeltaTime, Speed);
	SetActorLocation(NewLocation);

}

void ACrowdActor::MoveTo(const FVector& NewTargetLocation) {
	SetAllStrengthToInitialValue();
	TargetLocation = NewTargetLocation;
	bShouldMove = true;
}

void ACrowdActor::SetAllStrengthToInitialValue(){
	NiagaraSystem->SetFloatParameter(FName("User.SpeedLimit"), InitialSpeedLimit);
	NiagaraSystem->SetFloatParameter(FName("User.DragStrength"), InitialDragStrength);
	NiagaraSystem->SetFloatParameter(FName("User.AttractionStrength"), InitialAttractionStrength);
	NiagaraSystem->SetVectorParameter(FName("User.GravityStrength"), InitialGravityStrength);
	NiagaraSystem->SetFloatParameter(FName("User.VortexStrength"), InitialVortexStrength);
	NiagaraSystem->SetFloatParameter(FName("User.NoiseStrength1"), InitialNoiseStrength1);
	NiagaraSystem->SetFloatParameter(FName("User.NoiseStrength2"), InitialNoiseStrength2);
}

void ACrowdActor::SetAllStrengthToZero() {
	NiagaraSystem->SetFloatParameter(FName("User.DragStrength"), 0.0f);
	NiagaraSystem->SetFloatParameter(FName("User.AttractionStrength"), 0.0f);
	NiagaraSystem->SetVectorParameter(FName("User.GravityStrength"), FVector(0.0f, 0.0f, 0.0f));
	NiagaraSystem->SetFloatParameter(FName("User.VortexStrength"), 0.0f);
	NiagaraSystem->SetFloatParameter(FName("User.NoiseStrength1"), 0.0f);
	NiagaraSystem->SetFloatParameter(FName("User.NoiseStrength2"), 0.0f);
}

AActor* ACrowdActor::GetTargetActor() { return TargetActor; }
UNiagaraComponent* ACrowdActor::GetNiagaraSystem(){ return NiagaraSystem; }

/*
float ACrowdActor::GetIniatialDragStrength() const { return InitialDragStrength; }
float ACrowdActor::GetInitialAttractionStrength() const { return InitialAttractionStrength; }
FVector ACrowdActor::GetInitialGravityStrength() const { return InitialGravityStrength; }
float ACrowdActor::GetInitialVortexStrength() const { return InitialVortexStrength; }
float ACrowdActor::GetInitialNoiseStrength1() const { return InitialNoiseStrength1; }
float ACrowdActor::GetInitialNoiseStrength2() const { return InitialNoiseStrength2; }
float ACrowdActor::GetInitialSpeedLimit() const { return InitialSpeedLimit; }
*/


void ACrowdActor::SetTargetActor(AActor* NewTarget) { TargetActor = NewTarget; }
void ACrowdActor::SetPingMarker(APingMarker* NewPingMarker) { PingMarker = NewPingMarker; }


