
#include "Gameplay/Swarm/Actors/SwarmPlaneActor.h"


ASwarmPlaneActor::ASwarmPlaneActor() {
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));
	RootComponent = PlaneMesh;

	PlaneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlaneMesh->SetCollisionProfileName(TEXT("BlockAll"));
}

void ASwarmPlaneActor::Activate(float Lifetime) {
	PlaneMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		this,
		&ASwarmPlaneActor::Expire,
		Lifetime,
		false
	);
}

void ASwarmPlaneActor::Expire() {
	Destroy();
}
