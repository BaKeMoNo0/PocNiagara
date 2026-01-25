#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/Player/PlayerCharacter.h"
#include "Gameplay/Swarm/CrowdActor.h"
#include "DarkSwarmGameMode.generated.h"


UCLASS()
class DARKSWARM_API ADarkSwarmGameMode : public AGameModeBase {
	GENERATED_BODY()
	
	UPROPERTY()
	APlayerCharacter* PendingDeadPlayer;
	
	
	
public:
	ADarkSwarmGameMode();
	
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<ACrowdActor> CrowdActorClass;
	UPROPERTY()
	ACrowdActor* CrowdActor;
	

public:
	void HandleRespawn();
	void OnPlayerDied(APlayerCharacter* DeadPlayer);
	
};
