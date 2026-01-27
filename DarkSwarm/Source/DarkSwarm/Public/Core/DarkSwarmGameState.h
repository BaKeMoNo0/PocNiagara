
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/Swarm/CrowdActor.h"
#include "DarkSwarmGameState.generated.h"


UCLASS()
class DARKSWARM_API ADarkSwarmGameState : public AGameStateBase {
	GENERATED_BODY()
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CrowdActor)
	ACrowdActor* CrowdActor = nullptr;
	
	UFUNCTION()
	void OnRep_CrowdActor();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCrowdActorReady, ACrowdActor*);
	FOnCrowdActorReady OnCrowdActorReady;
	
	ACrowdActor* GetCrowdActor() const;
	void SetCrowdActor(ACrowdActor* NewCrowdActor);
	
};
