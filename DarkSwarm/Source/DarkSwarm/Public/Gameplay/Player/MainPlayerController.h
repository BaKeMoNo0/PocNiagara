#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputAction.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/Swarm/CrowdActor.h"
#include "MainPlayerController.generated.h"


class ADisintegratableActor;

UCLASS()
class DARKSWARM_API AMainPlayerController : public APlayerController {
	GENERATED_BODY()

	UPROPERTY()
	APlayerCharacter* ControlledCharacter;
	UPROPERTY()
	ACrowdActor* Swarm;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void HandleCrowdActorReady(ACrowdActor* InCrowdActor);
	

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY()
	UUserWidget *MainMenuWidget;

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
	TSet<TWeakObjectPtr<ADisintegratableActor>> NearbyDisintegratables;
	
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
	void TriggerDisintegration();
	void CallFoostep(const FInputActionValue& Value);
	
	void InitWidget();
	
	UFUNCTION()
	void HandleDisintegrationStarted(ADisintegratableActor* Source);
	
	UFUNCTION()
	void OnEnterInteractable(ADisintegratableActor* Source, AActor* Interactor);

	UFUNCTION()
	void OnExitInteractable(ADisintegratableActor* Source, AActor* Interactor);

};
