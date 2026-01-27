
#include "Core/DarkSwarmGameState.h"
#include "Net/UnrealNetwork.h"

void ADarkSwarmGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADarkSwarmGameState, CrowdActor);
}

void ADarkSwarmGameState::OnRep_CrowdActor() {
	if (!CrowdActor) return;
	UE_LOG(LogTemp, Log, TEXT("CrowdActor ready"));
	OnCrowdActorReady.Broadcast(CrowdActor);
}


ACrowdActor* ADarkSwarmGameState::GetCrowdActor() const { return CrowdActor; }

void ADarkSwarmGameState::SetCrowdActor(ACrowdActor* NewCrowdActor) {
	if (CrowdActor == NewCrowdActor) return;
	CrowdActor = NewCrowdActor;
	OnRep_CrowdActor();
}
