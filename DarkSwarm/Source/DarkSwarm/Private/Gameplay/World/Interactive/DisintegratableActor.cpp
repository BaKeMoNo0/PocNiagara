#include "Gameplay/World/Interactive/DisintegratableActor.h"
#include "NiagaraComponent.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/Systems/Desintegration/DisintegratableComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	WidgetAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetAnchor"));
	WidgetAnchor->SetupAttachment(RootComponent);
	
	InteractWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidgetComp->SetupAttachment(WidgetAnchor);
	
	InteractWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	InteractWidgetComp->SetPivot(FVector2D(0.5f, 0.5f));
	InteractWidgetComp->SetDrawAtDesiredSize(true);
	InteractWidgetComp->SetVisibility(false);
	
	DisintegratableComp = CreateDefaultSubobject<UDisintegratableComponent>(TEXT("DisintegratableComp"));
}


void ADisintegratableActor::BeginPlay() {
	Super::BeginPlay();
}

void ADisintegratableActor::UpdateWidgetFacingCamera() {
	if (!InteractWidgetComp) return;

	APlayerCameraManager* Cam = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (!Cam) return;

	const FVector ToCamera = Cam->GetCameraLocation() - InteractWidgetComp->GetComponentLocation();
	FRotator LookAtRot = ToCamera.Rotation();
	LookAtRot.Pitch = 0.f;
	LookAtRot.Roll  = 0.f;

	WidgetAnchor->SetWorldRotation(LookAtRot);
}


void ADisintegratableActor::HandleEnterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!OtherActor || OtherActor == this) return;
	
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) {
		InteractWidgetComp->SetVisibility(true);
		UpdateWidgetFacingCamera();
	}
	OnInteractorEnter.Broadcast(this, OtherActor);
	
}

void ADisintegratableActor::HandleExitOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!OtherActor || OtherActor == this) return;
	
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()){
		InteractWidgetComp->SetVisibility(false);
	}
	OnInteractorExit.Broadcast(this, OtherActor);
}
