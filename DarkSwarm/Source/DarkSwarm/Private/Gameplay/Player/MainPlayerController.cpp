
#include "Gameplay/Player/MainPlayerController.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Core/DarkSwarmGameState.h"
#include "Gameplay/Player/Components/PlayerMovementComponent.h"
#include "Gameplay/Player/Components/PlayerPingComponent.h"
#include "Gameplay/Player/Components/PlayerSoundComponent.h"
#include "Gameplay/Systems/Desintegration//DisintegratableComponent.h"
#include "Gameplay/World/Interactive//DisintegratableActor.h"


void AMainPlayerController::BeginPlay() {
	Super::BeginPlay();

	InitWidget();
	if (ADarkSwarmGameState* GS = GetWorld()->GetGameState<ADarkSwarmGameState>()) {
		GS->OnCrowdActorReadyEvent().AddUObject(this,&AMainPlayerController::HandleCrowdActorReady);

		if (ACrowdActor* Existing = GS->GetCrowdActor()) HandleCrowdActorReady(Existing);
	}
	
	for (TActorIterator<ADisintegratableActor> It(GetWorld()); It; ++It) {
		ADisintegratableActor* Actor = *It;
		if (!Actor) continue;


		Actor->OnInteractorEnterEvent().AddDynamic(this, &AMainPlayerController::OnEnterInteractable);
		Actor->OnInteractorExitEvent().AddDynamic(this, &AMainPlayerController::OnExitInteractable);
		Actor->GetDisintegratableComponent()->OnDisintegrationStartedEvent().AddDynamic(this, &AMainPlayerController::HandleDisintegrationStarted);
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

		EnhancedInputComponent->BindAction(DesintegrationAction, ETriggerEvent::Triggered, this, &AMainPlayerController::TriggerDisintegration);
		
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
	Swarm->SetSwarmForm(ESwarmForm::Cube);
}

void AMainPlayerController::SetFormPlane() {
	Swarm->SetSwarmForm(ESwarmForm::Plane);
}



//Disintegration

void AMainPlayerController::HandleDisintegrationStarted(ADisintegratableActor* Source) {
	if (!Swarm) return;
	Swarm->AbsorbDisintegratedActor(Source);
}


void AMainPlayerController::TriggerDisintegration() {
	UE_LOG(LogTemp, Warning, TEXT("TriggerDisintegration called. Count = %d"), NearbyDisintegratables.Num());

	if (NearbyDisintegratables.IsEmpty()) return;
	
	for (const TWeakObjectPtr<ADisintegratableActor>& WeakActor : NearbyDisintegratables) {
		if (!WeakActor.IsValid()) continue;
		
		if (UDisintegratableComponent* DisComponent = WeakActor->FindComponentByClass<UDisintegratableComponent>()) {
			DisComponent->TriggerDisintegration();
		}
	}
}


void AMainPlayerController::OnEnterInteractable(ADisintegratableActor* Source, AActor* Interactor) {
	if (Interactor != GetPawn()) return;
	
	UE_LOG(LogTemp, Warning, TEXT("ENTER overlap with %s"), *Source->GetName());

	NearbyDisintegratables.Add(Source);
	
	// UI : "Appuyer sur E"
}

void AMainPlayerController::OnExitInteractable(ADisintegratableActor* Source, AActor* Interactor) {
	if (Interactor != GetPawn()) return;

	UE_LOG(LogTemp, Warning, TEXT("Exit overlap with %s"), *Source->GetName());
	
	NearbyDisintegratables.Remove(Source);
	// UI off
}



void AMainPlayerController::CallFoostep(const FInputActionValue& Value) {
	if (!ControlledCharacter) return;
	
	const float Axis = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("Footstep Axis = %f"), Axis);
	
	if (FMath::IsNearlyZero(Axis)) return;
	const bool bIsLeftFoot = Axis < 0.f;
	
	ControlledCharacter->TrySpawnFootStep(bIsLeftFoot);
}
