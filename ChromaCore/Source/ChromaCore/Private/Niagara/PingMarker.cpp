
#include "Niagara/PingMarker.h"

#include "Components/WidgetComponent.h"


APingMarker::APingMarker() {
	PrimaryActorTick.bCanEverTick = false;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(RootComponent);
	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void APingMarker::SetFormVisual(EFormType Type) {
	if (PingMaterial) VisualMesh->SetMaterial(0, PingMaterial);
	
	switch (Type) {
	case EFormType::Cube:
		VisualMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube")));
		VisualMesh->SetWorldScale3D(FVector(1.0f));
		break;
	case EFormType::Plane:
		VisualMesh->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Plane")));
		VisualMesh->SetWorldScale3D(FVector(1.5f));
		break;
	default:
		break;
	}
}
