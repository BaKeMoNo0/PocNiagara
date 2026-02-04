#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractionComponent.generated.h"


class ADisintegratableActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSWARM_API UPlayerInteractionComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UPlayerInteractionComponent();
	
private:
	UPROPERTY()
	TSet<TWeakObjectPtr<ADisintegratableActor>> NearbyInteractables;

public:
	void TriggerPrimaryInteraction();
	
	UFUNCTION()
	void RegisterInteractable(ADisintegratableActor* Actor, AActor* Interactor);
	UFUNCTION()
	void UnregisterInteractable(ADisintegratableActor* Actor, AActor* Interactor);

	
		
};
