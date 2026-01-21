#include "Gameplay/Triggers/DeathBox.h"

#include "Gameplay/Player/PlayerCharacter.h"

ADeathBox::ADeathBox() {}

void ADeathBox::BeginPlay() {
	Super::BeginPlay();
	
}

void ADeathBox::OnPlayerTriggered(APlayerCharacter* Player) {
	if (!Player) return;
	Player->Die();
}



