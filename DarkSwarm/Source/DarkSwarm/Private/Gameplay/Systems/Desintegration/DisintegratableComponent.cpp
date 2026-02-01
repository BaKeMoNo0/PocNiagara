#include "Gameplay/Systems/Desintegration/DisintegratableComponent.h"
#include "Gameplay/World/Interactive/DisintegratableActor.h"
#include "NiagaraComponent.h"



UDisintegratableComponent::UDisintegratableComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}


void UDisintegratableComponent::BeginPlay() {
	Super::BeginPlay();
	
	OwnerActor = Cast<ADisintegratableActor>(GetOwner());
	if (OwnerActor && OwnerActor->GetDisintegratingMesh()) {
		DynMat = OwnerActor->GetDisintegratingMesh()->CreateAndSetMaterialInstanceDynamic(0);
		if (DynMat) DynMat->SetScalarParameterValue("Opacity", 1.0f);
	}
}


void UDisintegratableComponent::TriggerDisintegration() {
	if (!OwnerActor) return;
	
	if (OwnerActor->GetNiagaraComp() && bAutoActivateNiagara) OwnerActor->GetNiagaraComp()->Activate(true);
	OnDisintegrationStarted.Broadcast(OwnerActor);
	
	FadeOutMesh();
	if (OwnerActor->GetDisintegratingMesh()) OwnerActor->GetDisintegratingMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (OwnerActor->GetSphereCollision()) OwnerActor->GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



void UDisintegratableComponent::FadeOutMesh() {
	if (!DynMat || !OwnerActor) return;

	float Opacity = 1.f;
	
	FTimerHandle LocalHandle;
	GetWorld()->GetTimerManager().SetTimer(
		LocalHandle, 
		FTimerDelegate::CreateLambda([this, &LocalHandle]() {
		static float LocalOpacity = 1.f;
		LocalOpacity = FMath::Clamp(LocalOpacity - FadeStep, 0.f, 1.f);
		DynMat->SetScalarParameterValue("Opacity", LocalOpacity);

		if (LocalOpacity <= 0.f) {
			GetWorld()->GetTimerManager().ClearTimer(LocalHandle);
			OwnerActor->GetDisintegratingMesh()->SetVisibility(false);
		}
	}),FadeInterval, true);
}

