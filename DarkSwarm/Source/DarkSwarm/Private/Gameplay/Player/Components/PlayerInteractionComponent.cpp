

#include "Gameplay/Player/Components/PlayerInteractionComponent.h"

#include "Gameplay/Systems/Desintegration/DisintegratableComponent.h"
#include "Gameplay/World/Interactive/DisintegratableActor.h"


UPlayerInteractionComponent::UPlayerInteractionComponent() {
	PrimaryComponentTick.bCanEverTick = false;

}

void UPlayerInteractionComponent::TriggerPrimaryInteraction() {
	if (NearbyInteractables.IsEmpty()) return;
	
	for (const TWeakObjectPtr<ADisintegratableActor>& WeakActor : NearbyInteractables) {
		if (!WeakActor.IsValid()) continue;
		
		if (UDisintegratableComponent* DisComponent = WeakActor->GetDisintegratableComponent()) {
			DisComponent->TriggerDisintegration();
		}
	}
}

void UPlayerInteractionComponent::RegisterInteractable(ADisintegratableActor* Actor, AActor* Interactor) {
	if (!Actor || Interactor != GetOwner()) return;
	NearbyInteractables.Add(Actor);
}

void UPlayerInteractionComponent::UnregisterInteractable(ADisintegratableActor* Actor, AActor* Interactor) {
	if (!Actor || Interactor != GetOwner()) return;
	NearbyInteractables.Remove(Actor);
}



