#include "Gameplay/Triggers/BaseTriggerBox.h"

ABaseTriggerBox::ABaseTriggerBox() {
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	RootComponent = BoxComponent;
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseTriggerBox::OnOverlapBegin);
	
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);

}

void ABaseTriggerBox::BeginPlay() {
	Super::BeginPlay();
	
}


void ABaseTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	OnPlayerTriggered(Player);
}


void ABaseTriggerBox::OnPlayerTriggered(APlayerCharacter* Player){ /* children will implement it*/ }

