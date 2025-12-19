
#include "Niagara/Footstep.h"

AFootstep::AFootstep() {
	PrimaryActorTick.bCanEverTick = false;
}

void AFootstep::BeginPlay() {
	Super::BeginPlay();
	SetLifeSpan(0.6f);
}

