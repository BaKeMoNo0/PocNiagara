#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DarkSwarmGameMode.generated.h"


class APlayerCharacter;
class ACrowdActor;

UCLASS()
class DARKSWARM_API ADarkSwarmGameMode : public AGameModeBase {
	GENERATED_BODY()
	
	// Player waiting for respawn after death & swarm consumption
	UPROPERTY()
	APlayerCharacter* PendingDeadPlayer;
	
	bool bIsRespawning = false;
	
public:
	ADarkSwarmGameMode();
	
protected:
	virtual void BeginPlay() override;
	
	// GameMode is responsible for spawning and owning the global Swarm system (CrowdActor).
	// The reference is exposed to other systems through the GameState.
	UPROPERTY(EditDefaultsOnly, Category = "Swarm")
	TSubclassOf<ACrowdActor> CrowdActorClass;
	UPROPERTY()
	ACrowdActor* CrowdActor;
	

public:
	void HandleRespawn();
	void OnPlayerDied(APlayerCharacter* DeadPlayer);
	
};
