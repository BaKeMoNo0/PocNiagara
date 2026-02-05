
#include "Gameplay/Player/MainPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Core/DarkSwarmGameState.h"
#include "Gameplay/Player/Components/PlayerInteractionComponent.h"
#include "Gameplay/Player/Components/PlayerMovementComponent.h"
#include "Gameplay/Player/Components/PlayerPingComponent.h"
#include "Gameplay/Player/Components/PlayerSoundComponent.h"
#include "Gameplay/Player/Components/SwarmAbilityComponent.h"


void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();

	InitWidget();
	if (ADarkSwarmGameState* GS = GetWorld()->GetGameState<ADarkSwarmGameState>()) {
		GS->OnCrowdActorReadyEvent().AddUObject(this,&AMainPlayerController::HandleCrowdActorReady);

		if (ACrowdActor* Existing = GS->GetCrowdActor()) HandleCrowdActorReady(Existing);
	}
}

void AMainPlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	
	ControlledCharacter = Cast<APlayerCharacter>(InPawn);
	if (!ControlledCharacter) UE_LOG(LogTemp, Error, TEXT("PlayerController possessed non PlayerCharacter pawn"));
}

void AMainPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(InputMappingContext,0);
	}

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(InputComponent);
	
	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleMove);
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleLook);

	EIC->BindAction(RunAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleStartRun);
	EIC->BindAction(RunAction, ETriggerEvent::Completed, this, &AMainPlayerController::HandleStopRun);

	EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleJump);

	EIC->BindAction(PingAction, ETriggerEvent::Started, this, &AMainPlayerController::HandleStartAiming);
	EIC->BindAction(PingAction, ETriggerEvent::Completed, this, &AMainPlayerController::HandleStopAiming);
	
	EIC->BindAction(CallBackAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleCallbackSwarm);
	
	EIC->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AMainPlayerController::AdjustPingDistance);
	EIC->BindAction(SoundAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleSound);

	EIC->BindAction(SelectCubeAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleSetFormCube);
	EIC->BindAction(SelectPlaneAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleSetFormPlane);

	EIC->BindAction(DesintegrationAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleTriggerDisintegration);
	
	EIC->BindAction(FootstepAction, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleFootstep);
	
	EIC->BindAction(CreatePlane, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleCreatePlane);
}




void AMainPlayerController::InitWidget() {
	if (MainMenuWidgetClass) {
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidget) {
			MainMenuWidget->AddToViewport();
			MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AMainPlayerController::HandleCreatePlane() {
	if (!ControlledCharacter) return;
    	
	ControlledCharacter->GetSwarmAbilityComponent()->TrySpawnPlane(ControlledCharacter);
}


void AMainPlayerController::HandleMove(const FInputActionValue &Value) {
	if (auto* Move = ControlledCharacter? ControlledCharacter->GetPlayerMovementComponent() : nullptr) {
		Move->CallMove(Value.Get<FVector2D>());
	}
}



void AMainPlayerController::HandleLook(const FInputActionValue &Value) {
	if (auto* Move = ControlledCharacter? ControlledCharacter->GetPlayerMovementComponent() : nullptr) {
		Move->CallLook(Value.Get<FVector2D>());
	}
}

void AMainPlayerController::HandleStartRun() {
	if (auto* Move = ControlledCharacter? ControlledCharacter->GetPlayerMovementComponent() : nullptr) {
		Move->CallRun();
	}
}

void AMainPlayerController::HandleStopRun() {
	if (auto* Move = ControlledCharacter? ControlledCharacter->GetPlayerMovementComponent() : nullptr) {
		Move->CallStopRunning();
	}
}

void AMainPlayerController::HandleJump() {
	if (auto* Move = ControlledCharacter? ControlledCharacter->GetPlayerMovementComponent() : nullptr) {
		Move->CallJump();
	}
}



//CrowdActor
void AMainPlayerController::HandleStartAiming() {
	if (auto* Ping = ControlledCharacter? ControlledCharacter->GetPlayerPingComponent() : nullptr) {
		Ping->StartAiming();
	}
}

void AMainPlayerController::HandleStopAiming() {
	if (auto* Ping = ControlledCharacter? ControlledCharacter->GetPlayerPingComponent() : nullptr) {
		Ping->StopAiming();
	}
}

void AMainPlayerController::HandleCallbackSwarm() {
	if (Swarm && ControlledCharacter) {
		Swarm->ReturnToPlayer(ControlledCharacter);
	}
}

void AMainPlayerController::HandleSound() {
	if (ControlledCharacter) ControlledCharacter->GetPlayerSoundComponent()->CallSound();
}


void AMainPlayerController::AdjustPingDistance(const FInputActionValue& Value) {
	if (ControlledCharacter && ControlledCharacter->GetPlayerPingComponent()) {
		ControlledCharacter->GetPlayerPingComponent()->AdjustPingDistance(Value.Get<float>());
	}
}

void AMainPlayerController::HandleSetFormCube() {
	if (Swarm) Swarm->SetSwarmForm(ESwarmForm::Cube);
}

void AMainPlayerController::HandleSetFormPlane() {
	if (Swarm) Swarm->SetSwarmForm(ESwarmForm::Plane);
}



//Disintegration
void AMainPlayerController::HandleTriggerDisintegration() {
	if (auto* Interaction = ControlledCharacter? ControlledCharacter->GetPlayerInteractionComponent() : nullptr) {
		Interaction->TriggerPrimaryInteraction();
	}
}



//Footstep
void AMainPlayerController::HandleFootstep(const FInputActionValue& Value) {
	if (!ControlledCharacter) return;
	
	const float Axis = Value.Get<float>();
	if (FMath::IsNearlyZero(Axis)) return;
	
	ControlledCharacter->GetSwarmAbilityComponent()->TrySpawnFootStep(ControlledCharacter, Axis < 0.f);
}

//
// ===== Crowd =====
//


void AMainPlayerController::HandleCrowdActorReady(ACrowdActor* InCrowdActor) {
	// Assign the Swarm HERE (don't remove)
	Swarm = InCrowdActor;
	
	if (ControlledCharacter) {
		Swarm->SetTargetActor(ControlledCharacter);
	}
}