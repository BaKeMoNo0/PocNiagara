// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/Component/PlayerMovementComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"


void UPlayerMovementComponent::BeginPlay() {
	Super::BeginPlay();
	OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerMovementComponent::CallMove(const FVector2D MovementVector) {
	if (OwnerCharacter == nullptr) return;

	AController* Controller = OwnerCharacter->GetController();
	if (!Controller) return;
	
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);
	
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	OwnerCharacter->AddMovementInput(ForwardVector, MovementVector.Y);
	OwnerCharacter->AddMovementInput(RightVector, MovementVector.X);
}

void UPlayerMovementComponent::CallLook(const FVector2D LookDirection) const {
	if (OwnerCharacter) {
		if (APlayerController *PlayerController = Cast<APlayerController>(OwnerCharacter->GetController())) {
			PlayerController->AddPitchInput(LookDirection.Y);
			PlayerController->AddYawInput(LookDirection.X);
		}
	}
}


void UPlayerMovementComponent::CallJump() const {
	if (OwnerCharacter) OwnerCharacter->Jump();
}


void UPlayerMovementComponent::CallRun() {
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}


void UPlayerMovementComponent::CallStopRunning() {
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
}