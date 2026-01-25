
#include "Core/DarkSwarmGameState.h"
#include "Net/UnrealNetwork.h"

void ADarkSwarmGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADarkSwarmGameState, CrowdActor);
}

void ADarkSwarmGameState::OnRep_CrowdActor() {
	UE_LOG(LogTemp, Warning, TEXT("CrowdActor replicated"));
}



ACrowdActor* ADarkSwarmGameState::GetCrowdActor() const { return CrowdActor; }

void ADarkSwarmGameState::SetCrowdActor(ACrowdActor* NewCrowdActor) { CrowdActor = NewCrowdActor; }
