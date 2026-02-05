
#include "Gameplay/Player/Components/SwarmAbilityComponent.h"

#include "Components/CapsuleComponent.h"
#include "Gameplay/Player/PlayerCharacter.h"
#include "Gameplay/Swarm/Actors/SwarmPlaneActor.h"

USwarmAbilityComponent::USwarmAbilityComponent() {
	PrimaryComponentTick.bCanEverTick = true;

}


void USwarmAbilityComponent::BeginPlay() {
	Super::BeginPlay();

}

void USwarmAbilityComponent::TrySpawnPlane(APlayerCharacter* Player) {
	if (!SwarmPlaneClass || !Player) return;

	FVector SpawnLocation = Player->GetActorLocation();
	SpawnLocation.Z -= Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.f;

	FRotator SpawnRotation = Player-> GetActorRotation();

	ASwarmPlaneActor* Plane = GetWorld()->SpawnActor<ASwarmPlaneActor>(
		SwarmPlaneClass,
		SpawnLocation,
		SpawnRotation
	);

	if (Plane) Plane->Activate(2.0f);
}


void USwarmAbilityComponent::TrySpawnFootStep(APlayerCharacter* Player, bool bIsLeftFoot) {
	//if (bIsLeftFoot != bExpectLeftFoot) return;

	if (!FootstepActorClass) return;

	const float CapsuleHalfHeight = Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector SpawnLocation = Player->GetActorLocation();
	SpawnLocation.Z -= CapsuleHalfHeight + 5.f;

	SpawnLocation += Player->GetActorForwardVector() * ForwardOffset;
	SpawnLocation += Player->GetActorRightVector() * (bIsLeftFoot ? -SideOffset : SideOffset);
	
	if (bIsLeftFoot) 
		Player->PlayAnimMontage(AnimStrideFootL, 0.4);
	else 
		Player->PlayAnimMontage(AnimStrideFootR, 0.4);
	
	Player->LaunchCharacter(
		Player->GetActorForwardVector() * StepImpulse,
		false,
		false
	);
	
	GetWorld()->SpawnActor<ASwarmPlatform>(
		FootstepActorClass,
		SpawnLocation,	
		FRotator::ZeroRotator
	);
	
	bExpectLeftFoot = !bExpectLeftFoot;
}

