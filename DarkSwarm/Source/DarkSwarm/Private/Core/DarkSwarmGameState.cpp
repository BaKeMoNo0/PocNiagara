
#include "Core/DarkSwarmGameState.h"
#include "Net/UnrealNetwork.h"
#include "Gameplay/Swarm/CrowdActor.h"



void ADarkSwarmGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADarkSwarmGameState, CrowdActor);
}


void ADarkSwarmGameState::NotifyCrowdActorReady() {
	if (!CrowdActor) return;
	OnCrowdActorReady.Broadcast(CrowdActor);
}

void ADarkSwarmGameState::OnRep_CrowdActor() { NotifyCrowdActorReady(); }


void ADarkSwarmGameState::SetCrowdActor(ACrowdActor* NewCrowdActor) {
	if (CrowdActor == NewCrowdActor) return;
	CrowdActor = NewCrowdActor;
	
	// Server-side immediate notification
	if (HasAuthority()) NotifyCrowdActorReady();
}
