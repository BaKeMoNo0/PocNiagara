#include "Core/DarkSwarmGameMode.h"

#include "Core/DarkSwarmGameState.h"


ADarkSwarmGameMode::ADarkSwarmGameMode(){}

void ADarkSwarmGameMode::BeginPlay() {
	Super::BeginPlay();
	
	CrowdActor = GetWorld()->SpawnActor<ACrowdActor>(CrowdActorClass);
	if (ADarkSwarmGameState* GS = GetGameState<ADarkSwarmGameState>()) GS->SetCrowdActor(CrowdActor);
}


void ADarkSwarmGameMode::HandleRespawn() {
	if (!PendingDeadPlayer) return;

	AController* Controller = PendingDeadPlayer->GetController();
	FVector RespawnLocation = PendingDeadPlayer->GetLastCheckpointLocation() + FVector(0,0,90);

	PendingDeadPlayer->Destroy();

	APlayerCharacter* NewPlayer = GetWorld()->SpawnActor<APlayerCharacter>(DefaultPawnClass, RespawnLocation,FRotator::ZeroRotator);

	if (Controller && NewPlayer) Controller->Possess(NewPlayer);
	
	if (ACrowdActor* Swarm = CrowdActor) Swarm->OnPlayerRespawn(NewPlayer);
	
	PendingDeadPlayer = nullptr;
}


void ADarkSwarmGameMode::OnPlayerDied(APlayerCharacter* DeadPlayer) {
	if (!DeadPlayer) return;
	
	PendingDeadPlayer = DeadPlayer;

	if (CrowdActor) {
		CrowdActor->ConsumePlayer(
			DeadPlayer,
			CrowdActor->OnConsumeFinished
		);
	}
}
