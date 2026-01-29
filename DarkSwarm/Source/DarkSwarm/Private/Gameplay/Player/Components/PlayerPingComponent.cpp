// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Player/Components/PlayerPingComponent.h"

#include "Core/DarkSwarmGameState.h"
#include "Gameplay/World/Interactive/PingMarker.h"
#include "Gameplay/Swarm/CrowdActor.h"


UPlayerPingComponent::UPlayerPingComponent(){
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


void UPlayerPingComponent::BeginPlay() {
	Super::BeginPlay();
	GS = GetWorld()->GetGameState<ADarkSwarmGameState>();
	
	if (GS && GS->GetCrowdActor()) {
		ACrowdActor* Crowd = GS->GetCrowdActor();

		Crowd->OnDestinationReachedEvent().RemoveAll(this);
		Crowd->OnDestinationReachedEvent().AddUObject(this, &UPlayerPingComponent::OnCrowdReachedDestination);
	}
}


void UPlayerPingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsAiming) {
		TraceFromCrosshair();
	}
}

void UPlayerPingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (GS && GS->GetCrowdActor()) GS->GetCrowdActor()->OnDestinationReachedEvent().RemoveAll(this);
	
	Super::EndPlay(EndPlayReason);
}


void UPlayerPingComponent::StartAiming() {
	bIsAiming = true;
	SetComponentTickEnabled(true);

	if (IsValid(ActivePingMarker)) {
		ActivePingMarker->Destroy();
		ActivePingMarker = nullptr;
	}

	
	ESwarmForm SwarmForm = ESwarmForm::Cube;
	if (GS && GS->GetCrowdActor()) SwarmForm = GS->GetCrowdActor()->GetSwarmForm();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	ActivePingMarker = GetWorld()->SpawnActor<APingMarker>(
		PingMarkerClass,
		LastValidLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (ActivePingMarker) ActivePingMarker->SetFormVisual(SwarmForm);
}


void UPlayerPingComponent::StopAiming() {
	bIsAiming = false;
	SetComponentTickEnabled(false);

	if (GS && GS->GetCrowdActor()) GS->GetCrowdActor()->MoveTo(LastValidLocation);
	
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
		
		if (IsValid(ActivePingMarker) && GS && GS->GetCrowdActor()) {
			Params.AddIgnoredActor(ActivePingMarker);
			Params.AddIgnoredActor(GS->GetCrowdActor());
			Params.AddIgnoredActors(GS->GetCrowdActor()->Children);
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

void UPlayerPingComponent::OnCrowdReachedDestination() {
	DestroyPingMarker();
}

