// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/Component/PlayerPingComponent.h"

#include "Niagara/PingMarker.h"
#include "PlayerCharacter/PlayerCharacter.h"


UPlayerPingComponent::UPlayerPingComponent(){
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPlayerPingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsAiming) {
		TraceFromCrosshair();
	}
}

void UPlayerPingComponent::CallPing() {
	if (bIsAiming){
		StartAiming();
	} else{
		StopAiming();
	}
}


void UPlayerPingComponent::StartAiming() {
	bIsAiming = true;
	SetComponentTickEnabled(true);

	if (IsValid(ActivePingMarker)) {
		ActivePingMarker->Destroy();
		ActivePingMarker = nullptr;
	}


	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	EFormType FormType = EFormType::Cube;
	if (PlayerCharacter && PlayerCharacter->GetCrowdActor()) FormType = PlayerCharacter->GetCrowdActor()->GetFormType();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	ActivePingMarker = GetWorld()->SpawnActor<APingMarker>(
		PingMarkerClass,
		LastValidLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (ActivePingMarker) ActivePingMarker->SetFormVisual(FormType);
}


void UPlayerPingComponent::StopAiming() {
	bIsAiming = false;
	SetComponentTickEnabled(false);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (PlayerCharacter && PlayerCharacter->GetCrowdActor()) {
		PlayerCharacter->GetCrowdActor()->MoveTo(LastValidLocation);
		PlayerCharacter->GetCrowdActor()->CurrentPingMarkerToDestroy = ActivePingMarker;
	}
}

void UPlayerPingComponent::TraceFromCrosshair() {
	APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController());
	if (!PC) return;

	int32 SizeX, SizeY;
	PC->GetViewportSize(SizeX, SizeY);
	FVector2D ScreenCenter(SizeX / 2.f, SizeY / 2.f);

	FVector WorldOrigin, WorldDirection;
	if (PC->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldOrigin, WorldDirection)) {
		FVector Start = WorldOrigin;
		FVector End = Start + WorldDirection * CurrentPingDistance;

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());
		
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
		if (IsValid(ActivePingMarker) && PlayerCharacter && PlayerCharacter->GetCrowdActor()) {
			Params.AddIgnoredActor(ActivePingMarker);
			Params.AddIgnoredActor(PlayerCharacter->GetCrowdActor());
			Params.AddIgnoredActors(PlayerCharacter->GetCrowdActor()->Children);
		}

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params)) {
			const FVector SurfaceNormal = Hit.Normal;
			const float OffsetDistance = 20.f;
			LastValidLocation = Hit.ImpactPoint + SurfaceNormal * OffsetDistance;
		} else {
			LastValidLocation = End;
		}

		if (IsValid(ActivePingMarker)) {
			ActivePingMarker->SetActorLocation(LastValidLocation);
		}
	}
}

void UPlayerPingComponent::AdjustPingDistance(float AxisValue) {
	CurrentPingDistance = FMath::Clamp(CurrentPingDistance + AxisValue * 50.f, MinPingDistance, MaxPingDistance);
}

void UPlayerPingComponent::DestroyPingMarker() {
	if (ActivePingMarker && !ActivePingMarker->IsPendingKillPending()) {
		ActivePingMarker->Destroy();
	}
}

bool UPlayerPingComponent::IsThisMyActiveMarker(APingMarker* Marker) const {
	return Marker != nullptr && Marker == ActivePingMarker;
}
