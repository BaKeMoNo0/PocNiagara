
#include "Gameplay/Triggers//CheckPoint.h"

#include "Gameplay/Player/PlayerCharacter.h"

ACheckPoint::ACheckPoint(){}


void ACheckPoint::BeginPlay() {
	Super::BeginPlay();
	
}

void ACheckPoint::OnPlayerTriggered(APlayerCharacter* Player) {
	if (!Player) return;
	if (bIsActivated) return;

	bIsActivated = true;
	Player->SetLastCheckpointLocation(GetActorLocation());
	
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Overlap is called in CheckPoint"));
}

