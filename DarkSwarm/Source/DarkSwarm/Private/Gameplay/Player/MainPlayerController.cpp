
#include "Gameplay/Player/MainPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Core/DarkSwarmGameMode.h"
#include "Core/DarkSwarmGameState.h"
#include "Gameplay/Player/Component/PlayerMovementComponent.h"
#include "Gameplay/Player/Component/PlayerPingComponent.h"
#include "Gameplay/Player/Component/PlayerSoundComponent.h"
#include "Gameplay/World/Interactive/DisintegratableComponent.h"
#include "Gameplay/World/Interactive//DesintegrationActor.h"


void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();

	InitWidget();
	if (ADarkSwarmGameState* GS = GetWorld()->GetGameState<ADarkSwarmGameState>()) {
		GS->OnCrowdActorReady.AddUObject(this,&AMainPlayerController::HandleCrowdActorReady);

		if (ACrowdActor* Existing = GS->GetCrowdActor()) HandleCrowdActorReady(Existing);
	}
}

void AMainPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			GetLocalPlayer())) {
		Subsystem->AddMappingContext(InputMappingContext,0);
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
		EnhancedInputComponent->BindAction(SoundAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallSound);

		EnhancedInputComponent->BindAction(SelectCubeAction, ETriggerEvent::Triggered, this, &AMainPlayerController::SetFormCube);
		EnhancedInputComponent->BindAction(SelectPlaneAction, ETriggerEvent::Triggered, this, &AMainPlayerController::SetFormPlane);

		EnhancedInputComponent->BindAction(DesintegrationAction, ETriggerEvent::Triggered, this, &AMainPlayerController::TriggerDesintegration);
		
		EnhancedInputComponent->BindAction(FootstepAction, ETriggerEvent::Triggered, this, &AMainPlayerController::CallFoostep);
	}
}


void AMainPlayerController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
	
	ControlledCharacter = Cast<APlayerCharacter>(InPawn);
	if (!ControlledCharacter) UE_LOG(LogTemp, Error, TEXT("PlayerController possessed non PlayerCharacter pawn"));
}


void AMainPlayerController::HandleCrowdActorReady(ACrowdActor* InCrowdActor) {
	Swarm = InCrowdActor;

	if (ControlledCharacter) {
		Swarm->SetTargetActor(ControlledCharacter);
		Swarm->SetPingComp(ControlledCharacter->GetPlayerPingComponent());
	}
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
	if (ControlledCharacter && Swarm) {
		Swarm->ReturnToPlayer(ControlledCharacter);
	}
}

void AMainPlayerController::CallSound() {
	if (ControlledCharacter) ControlledCharacter->GetPlayerSoundComponent()->CallSound();
}


void AMainPlayerController::AdjustPingDistance(const FInputActionValue& Value) {
	if (ControlledCharacter && ControlledCharacter->GetPlayerPingComponent()) {
		ControlledCharacter->GetPlayerPingComponent()->AdjustPingDistance(Value.Get<float>());
	}
}

void AMainPlayerController::SetFormCube() {
	Swarm->SetFormType(EFormType::Cube);
}

void AMainPlayerController::SetFormPlane() {
	Swarm->SetFormType(EFormType::Plane);
}



void AMainPlayerController::TriggerDesintegration() {
	if (CurrentTargetActor) {
		if (UDisintegratableComponent* DisComponent = CurrentTargetActor->FindComponentByClass<UDisintegratableComponent>()) {
			DisComponent->TriggerDisintegration();
			CurrentTargetActor = nullptr;
		}
	}
}

void AMainPlayerController::CallFoostep(const FInputActionValue& Value) {
	if (!ControlledCharacter) return;
	
	const float Axis = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Footstep Axis = %f"), Axis);
	
	if (FMath::IsNearlyZero(Axis)) return;
	const bool bIsLeftFoot = Axis < 0.f;
	
	ControlledCharacter->TrySpawnFootStep(bIsLeftFoot);
}

ADesintegrationActor* AMainPlayerController::GetCurrentTargetActor() const { return CurrentTargetActor; }
void AMainPlayerController::SetCurrentTargetActor(ADesintegrationActor* Target) { CurrentTargetActor = Target; }
void AMainPlayerController::ClearCurrentTargetActor() { CurrentTargetActor = nullptr; }