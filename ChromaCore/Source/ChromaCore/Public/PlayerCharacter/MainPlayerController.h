// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputAction.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"


class ADesintegrationActor;

UCLASS()
class CHROMACORE_API AMainPlayerController : public APlayerController {
	GENERATED_BODY()

	UPROPERTY()
	APlayerCharacter* ControlledCharacter;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
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

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *PingAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *CallBackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *ScrollAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *SoundAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *SelectCubeAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *SelectPlaneAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *DesintegrationAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *FootstepAction;
	
	UPROPERTY()
	ADesintegrationActor* CurrentTargetActor = nullptr;
	
	void CallMove(const FInputActionValue &Value);
	void CallRun();
	void CallStopRunning();
	void CallJump();
	void CallLook(const FInputActionValue &Value);
	void CallStartAiming();
	void CallStopAiming();
	void CallBackActor();
	void CallSound();
	void AdjustPingDistance(const FInputActionValue& Value);
	void SetFormCube();
	void SetFormPlane();
	void TriggerDesintegration();
	void CallFoostep();
	
	void InitWidget();

public:
	ADesintegrationActor* GetCurrentTargetActor() const;
	void SetCurrentTargetActor(ADesintegrationActor* Target);
	void ClearCurrentTargetActor();
};
