#include "Niagara/DesintegrationActor.h"
#include "NiagaraComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"
#include "PlayerCharacter/MainPlayerController.h"

ADesintegrationActor::ADesintegrationActor() {
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	SphereComp->InitSphereRadius(200.0f);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ADesintegrationActor::OnPlayerEnterRadius);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ADesintegrationActor::OnPlayerExitRadius);


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraSystem->SetupAttachment(RootComponent);
	NiagaraSystem->SetAutoActivate(false);
}


void ADesintegrationActor::BeginPlay() {
	Super::BeginPlay();
	
}


void ADesintegrationActor::OnPlayerEnterRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor)) {
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(Player->GetController())) {
			PC->SetCurrentTargetActor(this);
			bPlayerIsNear = true;
			PlayerCharacter = Player;
		}
	}
}

void ADesintegrationActor::OnPlayerExitRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor)) {
		if (AMainPlayerController* PC = Cast<AMainPlayerController>(Player->GetController())) {
			if (PC->GetCurrentTargetActor() == this) {
				PC->ClearCurrentTargetActor();
			}
			bPlayerIsNear = false;
		}
	}
}

void ADesintegrationActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

bool ADesintegrationActor::GetPlayerIsNear() const { return bPlayerIsNear; }
UNiagaraComponent* ADesintegrationActor::GetNiagaraComp() const { return NiagaraSystem; }
UStaticMeshComponent* ADesintegrationActor::GetMesh() const { return Mesh; }
USphereComponent* ADesintegrationActor::GetSphereCollision() const { return SphereComp; }

APlayerCharacter* ADesintegrationActor::GetPlayerCharacter() const { return PlayerCharacter;}

