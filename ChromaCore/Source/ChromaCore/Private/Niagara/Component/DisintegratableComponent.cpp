#include "Niagara/Component/DisintegratableComponent.h"
#include "Niagara/DesintegrationActor.h"
#include "NiagaraComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"

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
		UNiagaraComponent *Niagara = Owner->GetNiagaraComp();
		if (Owner->GetNiagaraComp()) {
			if (Owner->GetPlayerCharacter()) {
				FVector CrowdLocation = Owner->GetPlayerCharacter()->GetCrowdActor()->GetActorLocation();
				DrawDebugSphere(GetWorld(), CrowdLocation, 20.f, 12, FColor::Green, false, 2.0f);
				Niagara->SetVectorParameter(FName("User.AttractionTarget"), CrowdLocation);	
			}
			Niagara->SetFloatParameter(FName("User.AttractionForce"), 5.0f);
			Niagara->SetFloatParameter(FName("User.NoiseForceDesintegration"), 50.0f);
			Owner->GetNiagaraComp()->Activate(true);

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(
				TimerHandle,
				[this, Niagara]() {
					Niagara->Deactivate();
				},
				5.0f,
				false
			);
		}
		
		if (Owner->GetMesh()) Owner->GetMesh()->SetVisibility(false);
	
		if (Owner->GetSphereCollision()) Owner->GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

