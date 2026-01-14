
#include "Gameplay/Swarm/SwarmPlatform.h"

ASwarmPlatform::ASwarmPlatform() {
	PrimaryActorTick.bCanEverTick = false;
}

void ASwarmPlatform::BeginPlay() {
	Super::BeginPlay();
	SetLifeSpan(0.6f);
}

