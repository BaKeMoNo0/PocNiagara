#include "Gameplay/World/Interactive/DisintegratableActor.h"
#include "NiagaraComponent.h"
#include "Gameplay/Systems/Desintegration/DisintegratableComponent.h"

ADisintegratableActor::ADisintegratableActor() {
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	InteractionSphere->SetupAttachment(RootComponent);
	
	InteractionSphere->InitSphereRadius(200.0f);
	InteractionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractionSphere->SetGenerateOverlapEvents(true);
	InteractionSphere->SetMobility(EComponentMobility::Movable);
	
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADisintegratableActor::HandleEnterOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ADisintegratableActor::HandleExitOverlap);

	DisintegratingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	DisintegratingMesh->SetupAttachment(RootComponent);
	DisintegratingMesh->SetCollisionProfileName(TEXT("BlockAll"));
	DisintegratingMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraSystem->SetupAttachment(RootComponent);
	NiagaraSystem->SetAutoActivate(false);
	
	DisintegratableComp = CreateDefaultSubobject<UDisintegratableComponent>(TEXT("DisintegratableComp"));
}


void ADisintegratableActor::BeginPlay() {
	Super::BeginPlay();
}


void ADisintegratableActor::HandleEnterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!OtherActor || OtherActor == this) return;
	OnInteractorEnter.Broadcast(this, OtherActor);
}

void ADisintegratableActor::HandleExitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!OtherActor || OtherActor == this) return;
	OnInteractorExit.Broadcast(this, OtherActor);
}
