
#include "Niagara/PingMarker.h"

#include "Components/WidgetComponent.h"


APingMarker::APingMarker() {
	PrimaryActorTick.bCanEverTick = false;
	MarkerWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MarkerWidget"));
	MarkerWidget->SetVisibility(true);
	MarkerWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MarkerWidget->SetRelativeRotation(FRotator::ZeroRotator);
	MarkerWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MarkerWidget->SetDrawAtDesiredSize(true);
	RootComponent = MarkerWidget;
}
