
#include "Gameplay/World/Interactive/PingMarker.h"


APingMarker::APingMarker() {
	PrimaryActorTick.bCanEverTick = false;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	RootComponent = VisualMesh;
	VisualMesh->SetupAttachment(RootComponent);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetMobility(EComponentMobility::Movable);
}

void APingMarker::BeginPlay() {
	Super::BeginPlay();
	
	//SetFormVisual(ESwarmForm::Cube);
}


void APingMarker::SetFormVisual(ESwarmForm Type) {
	if (PingMaterial) VisualMesh->SetMaterial(0, PingMaterial);
	
	switch (Type) {
	case ESwarmForm::Cube:
		VisualMesh->SetStaticMesh(CubeMesh);
		VisualMesh->SetWorldScale3D(FVector(1.0f));
		break;
	case ESwarmForm::Plane:
		VisualMesh->SetStaticMesh(PlaneMesh);
		VisualMesh->SetWorldScale3D(FVector(1.5f));
		break;
	default:
		break;
	}
}
