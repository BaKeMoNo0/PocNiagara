#include "Core/DarkSwarmGameMode.h"
#include "Core/DarkSwarmGameState.h"
#include "Gameplay/Player/PlayerCharacter.h"
#include "Gameplay/Swarm/CrowdActor.h"


ADarkSwarmGameMode::ADarkSwarmGameMode(){}

void ADarkSwarmGameMode::BeginPlay() {
	Super::BeginPlay();
	
	if (!ensure(CrowdActorClass)) return;
	
	// Spawn the global Swarm actor used throughout the game session
	CrowdActor = GetWorld()->SpawnActor<ACrowdActor>(CrowdActorClass);
	ensure(CrowdActor);
	if (ADarkSwarmGameState* GS = GetGameState<ADarkSwarmGameState>()) GS->SetCrowdActor(CrowdActor);
}


void ADarkSwarmGameMode::HandleRespawn() {
	if (!PendingDeadPlayer){
		bIsRespawning = false;
		return;
	}

	AController* Controller = PendingDeadPlayer->GetController();
	FVector RespawnLocation = PendingDeadPlayer->GetLastCheckpointLocation() + FVector(0,0,90);

	// Spawn the new player first
	APlayerCharacter* NewPlayer = GetWorld()->SpawnActor<APlayerCharacter>(DefaultPawnClass, RespawnLocation,FRotator::ZeroRotator);
	if (!NewPlayer) { // Critical failure: keep the old pawn alive
		bIsRespawning = false;
		return;
	}
	
	// Possess the new pawn
	if (Controller) Controller->Possess(NewPlayer);
	
	// Notify swarm AFTER new player exists
	if (CrowdActor) CrowdActor->OnPlayerRespawn(NewPlayer);
	
	// Now it's safe to destroy the old pawn
	PendingDeadPlayer->Destroy();
	PendingDeadPlayer = nullptr;
	bIsRespawning = false;
}


void ADarkSwarmGameMode::OnPlayerDied(APlayerCharacter* DeadPlayer) {
	if (!DeadPlayer || bIsRespawning) return;
	
	bIsRespawning = true;
	PendingDeadPlayer = DeadPlayer;

	if (CrowdActor) {
		CrowdActor->ConsumePlayer(
			DeadPlayer,
			CrowdActor->OnConsumeFinished
		);
	}
}
