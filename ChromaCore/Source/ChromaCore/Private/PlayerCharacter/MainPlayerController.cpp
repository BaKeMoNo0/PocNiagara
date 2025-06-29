// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/MainPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter/Component/PlayerMovementComponent.h"
#include "PlayerCharacter/Component/PlayerPingComponent.h"

void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();

	ControlledCharacter = Cast<APlayerCharacter>(GetPawn());
	InitWidget();
}

void AMainPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			GetLocalPlayer())) {
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallMove);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AMainPlayerController::CallRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AMainPlayerController::CallStopRunning);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallLook);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallJump);

		EnhancedInputComponent->BindAction(PingAction, ETriggerEvent::Started, this, &AMainPlayerController::CallStartAiming);
		EnhancedInputComponent->BindAction(PingAction, ETriggerEvent::Completed, this, &AMainPlayerController::CallStopAiming);
		EnhancedInputComponent->BindAction(CallBackAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallBackActor);
		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AMainPlayerController::AdjustPingDistance);
	}
}


void AMainPlayerController::InitWidget() {
	if (MainMenuWidgetClass) {
		if (UUserWidget *MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass)) {
			if (MainMenuWidget) {
				MainMenuWidget->AddToViewport();
				MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}


void AMainPlayerController::CallMove(const FInputActionValue &Value) {
	if (ControlledCharacter && ControlledCharacter->GetPlayerMovementComponent()) {
		ControlledCharacter->GetPlayerMovementComponent()->CallMove(Value.Get<FVector2D>());
	}
}



void AMainPlayerController::CallLook(const FInputActionValue &Value) {
	if (ControlledCharacter && ControlledCharacter->GetPlayerMovementComponent()) {
		ControlledCharacter->GetPlayerMovementComponent()->CallLook(Value.Get<FVector2D>());
	}
}

void AMainPlayerController::CallRun() {
	if (ControlledCharacter && ControlledCharacter->GetPlayerMovementComponent()) {
		ControlledCharacter->GetPlayerMovementComponent()->CallRun();
	}
}

void AMainPlayerController::CallStopRunning() {
	if (ControlledCharacter && ControlledCharacter->GetPlayerMovementComponent()) {
		ControlledCharacter->GetPlayerMovementComponent()->CallStopRunning();
	}
}

void AMainPlayerController::CallJump() {
	if (ControlledCharacter && ControlledCharacter->GetPlayerMovementComponent()) {
		ControlledCharacter->GetPlayerMovementComponent()->CallJump();
	}
}


void AMainPlayerController::CallStartAiming() {
	if (ControlledCharacter && ControlledCharacter->GetPlayerPingComponent()) {
		ControlledCharacter->GetPlayerPingComponent()->StartAiming();
	}
}

void AMainPlayerController::CallStopAiming() {
	if (ControlledCharacter && ControlledCharacter->GetPlayerPingComponent()) {
		ControlledCharacter->GetPlayerPingComponent()->StopAiming();
	}
}

void AMainPlayerController::CallBackActor() {
	if (ControlledCharacter) ControlledCharacter->GetCrowdActor()->SetTargetActor(ControlledCharacter);
}

void AMainPlayerController::AdjustPingDistance(const FInputActionValue& Value) {
	if (ControlledCharacter && ControlledCharacter->GetPlayerPingComponent()) {
		ControlledCharacter->GetPlayerPingComponent()->AdjustPingDistance(Value.Get<float>());
	}
}
