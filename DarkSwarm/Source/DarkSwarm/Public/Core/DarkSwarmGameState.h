
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DarkSwarmGameState.generated.h"

class ACrowdActor;


UCLASS()
class DARKSWARM_API ADarkSwarmGameState : public AGameStateBase {
	GENERATED_BODY()
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCrowdActorReady, ACrowdActor*);
	FOnCrowdActorReady OnCrowdActorReady;
	
	void NotifyCrowdActorReady();
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CrowdActor)
	ACrowdActor* CrowdActor = nullptr;
	
	UFUNCTION()
	void OnRep_CrowdActor();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	FOnCrowdActorReady& OnCrowdActorReadyEvent() { return OnCrowdActorReady; }
	
	ACrowdActor* GetCrowdActor() const;
	bool HasCrowdActor() const;

	void SetCrowdActor(ACrowdActor* NewCrowdActor);
	
};
