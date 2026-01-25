
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/Swarm/CrowdActor.h"
#include "DarkSwarmGameState.generated.h"


UCLASS()
class DARKSWARM_API ADarkSwarmGameState : public AGameStateBase {
	GENERATED_BODY()
	
protected:
	UPROPERTY(Replicated)
	ACrowdActor* CrowdActor;
	
	UFUNCTION()
	void OnRep_CrowdActor();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	ACrowdActor* GetCrowdActor() const;
	void SetCrowdActor(ACrowdActor* NewCrowdActor);
	
};
