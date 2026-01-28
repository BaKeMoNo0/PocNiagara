#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/SwarmForm.h"
#include "Types/CrowdState.h"
#include "Types/CrowdVisualParams.h"
#include "CrowdActor.generated.h"

class UCrowdVisualComponent;
class UNiagaraComponent;
class APlayerCharacter;
class UPlayerPingComponent;
class APingMarker;


UCLASS()
class DARKSWARM_API ACrowdActor : public AActor {
	GENERATED_BODY()
	
public:
	ACrowdActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void TickIdle(float);
	void TickFollowingPlayer(float);
	void TickMovingToTarget(float);
	void TickSlowingDown(float);
	
	void TickConsumingPlayer(float); //todo
	void TickReforming(float); //todo

	void OnEnterState(ECrowdState NewState);
	void OnExitState(ECrowdState OldState);
	void OnConsumeFXFinished(); //todo
	
	void MoveTowardsDestination(float DeltaTime);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* CollisionMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraSystem;
	
	
	UPROPERTY(VisibleAnywhere)
	UCrowdVisualComponent* VisualComp;
	
	UPROPERTY()
	UPlayerPingComponent* PingComp;
	
	
	UPROPERTY(EditDefaultsOnly, Category="Swarm|Visual")
	FCrowdVisualParams RestVisualParams;

	UPROPERTY(EditDefaultsOnly, Category="Swarm|Visual")
	FCrowdVisualParams ActionVisualParams;

	UPROPERTY(EditDefaultsOnly, Category="Swarm|Visual")
	float ParticleSpacing = 15.f;
	
	
	UPROPERTY(EditDefaultsOnly, Category="Swarm|Follow")
	FVector FollowOffset = FVector(-200.f, -180.f, 180.f);
	
	UPROPERTY(EditDefaultsOnly, Category = "Swarm|Follow")
	float FollowSpeed = 2.0f;
	
	
	float CurrentBlendAlpha = 0.0f;
	ESwarmForm SwarmForm;
	ECrowdState CrowdState;
	
	
	DECLARE_MULTICAST_DELEGATE(FOnSwarmConsumeFinished);
	FOnSwarmConsumeFinished OnConsumeFinished;
	
	
	UPROPERTY()
	AActor* TargetActor = nullptr;
	FVector TargetLocation;
	FVector Destination;
	
public:
	UPROPERTY()
	APingMarker* CurrentPingMarkerToDestroy = nullptr;
	
	FOnSwarmConsumeFinished& OnConsumeFinishedEvent() { return OnConsumeFinished; }
	void ConsumePlayer(APlayerCharacter* Player);
	
	void OnPlayerRespawn(AActor* NewPlayer);
	
	void MoveTo(const FVector& NewTargetLocation);
	void ReturnToPlayer(APlayerCharacter* Player);
	

	AActor* GetTargetActor() const { return TargetActor; }
	ESwarmForm GetSwarmForm() const { return SwarmForm; }
	UStaticMeshComponent* GetSphereMesh() const { return SphereMesh; }
	UNiagaraComponent* GetNiagaraSystem() const{ return NiagaraSystem; }
	
	const FCrowdVisualParams& GetRestVisualParams() const { return RestVisualParams; }
	const FCrowdVisualParams& GetActionVisualParams() const { return ActionVisualParams; }

	void SetPingComp(UPlayerPingComponent* PingCompRef) { PingComp = PingCompRef; }
	void SetTargetActor(AActor* NewTarget);
	void SetCrowdState(ECrowdState NewState);
	void SetActionParticleCount(int NewCount);
	void SetSwarmForm(ESwarmForm NewSwarmForm);
};