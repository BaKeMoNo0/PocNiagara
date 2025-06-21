// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputAction.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHROMACORE_API AMainPlayerController : public APlayerController {
	GENERATED_BODY()

	UPROPERTY()
	APlayerCharacter* ControlledCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	//UPROPERTY(EditAnywhere, Category = "UI")
	//TSubclassOf<UUserWidget> MainMenuWidgetClass;
	//UMainMenuWidget *MainMenuWidget;

	// Binding function for movement
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext *InputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *RunAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *JumpAction;
	
	void CallMove(const FInputActionValue &Value);
	void CallRun();
	void CallStopRunning();
	void CallJump();
	void CallLook(const FInputActionValue &Value);
};
