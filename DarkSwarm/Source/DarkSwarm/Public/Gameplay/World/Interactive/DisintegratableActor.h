#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "DisintegratableActor.generated.h"

class ACrowdActor;
class UWidgetComponent;
class UDisintegratableComponent;
class UNiagaraComponent;

/**
 * Actor that can be absorbed by the swarm.
 * Owns the interaction volume and delegates the actual disintegration
 * logic to its DisintegratableComponent.
 *
 * Responsibility:
 * - Detect player proximity
 * - Broadcast interaction events
 * - Remain mostly passive
 */


UCLASS()
class DARKSWARM_API ADisintegratableActor : public AActor {
	GENERATED_BODY()
	
private:
	void HandleCrowdReady(ACrowdActor* Crowd);
	
public:
	ADisintegratableActor();

protected:
	virtual void BeginPlay() override;
	
	void UpdateWidgetFacingCamera();
	
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	USceneComponent* WidgetAnchor;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	UWidgetComponent* InteractWidgetComp;
	
	
	UPROPERTY()
	UDisintegratableComponent* DisintegratableComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DisintegratingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraSystem;

	// Pure interaction volume.
	// Never blocks movement, only used to detect player proximity.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* InteractionSphere;

	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
		FOnInteractorSignature, 
		ADisintegratableActor*, Source, 
		AActor*, Interactor
		);
	
	UPROPERTY(BlueprintAssignable)
	FOnInteractorSignature OnInteractorEnter;

	UPROPERTY(BlueprintAssignable)
	FOnInteractorSignature OnInteractorExit;
	
	/**
	 * Called when an actor enters the interaction sphere.
	 * Only forwards the event; no gameplay logic should live here.
	 */
	UFUNCTION()
	void HandleEnterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							 bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleExitOverlap (UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	UNiagaraComponent* GetNiagaraComp() const { return NiagaraSystem; }
	UStaticMeshComponent* GetDisintegratingMesh() const { return DisintegratingMesh; }
	USphereComponent* GetSphereCollision() const { return InteractionSphere; }
	UDisintegratableComponent* GetDisintegratableComponent() const { return DisintegratableComp; }
	
	FOnInteractorSignature& OnInteractorEnterEvent() { return OnInteractorEnter; }
	FOnInteractorSignature& OnInteractorExitEvent() { return OnInteractorExit; }
};
