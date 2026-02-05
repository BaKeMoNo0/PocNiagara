#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputAction.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/Swarm/CrowdActor.h"
#include "MainPlayerController.generated.h"


UCLASS()
class DARKSWARM_API AMainPlayerController : public APlayerController {
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	void HandleMove(const FInputActionValue& Value);
	void HandleLook(const FInputActionValue& Value);

	void HandleStartRun();
	void HandleStopRun();
	void HandleJump();

	void HandleStartAiming();
	void HandleStopAiming();

	void HandleCallbackSwarm();
	void HandleSetFormCube();
	void HandleSetFormPlane();

	void HandleTriggerDisintegration();
	void HandleFootstep(const FInputActionValue& Value);
	
	void HandleSound();
	void AdjustPingDistance(const FInputActionValue& Value);
	
	void InitWidget();
	void HandleCreatePlane();

	// ===== Crowd =====
	UFUNCTION()
	void HandleCrowdActorReady(ACrowdActor* InCrowdActor);
	
	// ==== UI ====
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY()
	UUserWidget *MainMenuWidget;

	// ===== References =====
	UPROPERTY()
	APlayerCharacter* ControlledCharacter = nullptr;

	UPROPERTY()
	ACrowdActor* Swarm = nullptr;
	
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
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction *CreatePlane;
};
