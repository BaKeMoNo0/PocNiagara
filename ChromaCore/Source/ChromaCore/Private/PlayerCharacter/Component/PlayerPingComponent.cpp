// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/Component/PlayerPingComponent.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Ping pressed - aiming: %s"), bIsAiming ? TEXT("true") : TEXT("false"));
	if (bIsAiming){
		StartAiming();
	} else{
		StopAiming();
	}
}


void UPlayerPingComponent::StartAiming() {
	bIsAiming = true;
	SetComponentTickEnabled(true);
}

void UPlayerPingComponent::StopAiming() {
	bIsAiming = false;
	SetComponentTickEnabled(false);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (PlayerCharacter && PlayerCharacter->GetCrowdActor()){
		PlayerCharacter->GetCrowdActor()->SetTargetActor(nullptr);
		PlayerCharacter->GetCrowdActor()->MoveTo(LastValidLocation);
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
		FVector End = Start + WorldDirection * 1000.f;

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, 0, 1.0f);
		
		if (Hit.bBlockingHit) {
			LastValidLocation = Hit.Location;
			DrawDebugSphere(GetWorld(), Hit.Location, 8.f, 12, FColor::Red, false, 1.f);
		}
	}
}
