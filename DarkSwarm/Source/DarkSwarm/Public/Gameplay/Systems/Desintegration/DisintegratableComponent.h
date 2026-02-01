#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DisintegratableComponent.generated.h"


class ADisintegratableActor;


/**
 * Handles the visual and physical disintegration of the actor.
 * This component is stateless regarding the swarm and does not
 * know who absorbs the result.
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSWARM_API UDisintegratableComponent : public UActorComponent {
	GENERATED_BODY()
	
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
		FOnDisintegrationFinished,
		ADisintegratableActor*, Source
	);
	UPROPERTY(BlueprintAssignable)
	FOnDisintegrationFinished OnDisintegrationStarted;
	
	UPROPERTY()
	ADisintegratableActor* OwnerActor;

	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Disintegration|Niagara")
	float FadeStep = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Disintegration|Niagara")
	float FadeInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Disintegration|Niagara")
	bool bAutoActivateNiagara = true;

public:
	UDisintegratableComponent();

protected:
	virtual void BeginPlay() override;
	
	void FadeOutMesh();

public:
	/**
	 * Starts the disintegration process.
	 * This is a one-way operation and should only be called once.
	 */
	void TriggerDisintegration();
	FOnDisintegrationFinished& OnDisintegrationStartedEvent() { return OnDisintegrationStarted; }
};
