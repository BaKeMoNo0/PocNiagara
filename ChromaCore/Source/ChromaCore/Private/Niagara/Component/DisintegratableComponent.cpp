#include "Niagara/Component/DisintegratableComponent.h"
#include "Niagara/DesintegrationActor.h"
#include "NiagaraComponent.h"

UDisintegratableComponent::UDisintegratableComponent() {
	PrimaryComponentTick.bCanEverTick = true;

}


void UDisintegratableComponent::BeginPlay() {
	Super::BeginPlay();
	
	Owner = Cast<ADesintegrationActor>(GetOwner());
}


void UDisintegratableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDisintegratableComponent::TriggerDisintegration() {
	if (Owner) {
		if (Owner->GetNiagaraComp()) Owner->GetNiagaraComp()->Activate(true);
		
		if (Owner->GetMesh()) Owner->GetMesh()->SetVisibility(false);
	
		if (Owner->GetSphereCollision()) Owner->GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

