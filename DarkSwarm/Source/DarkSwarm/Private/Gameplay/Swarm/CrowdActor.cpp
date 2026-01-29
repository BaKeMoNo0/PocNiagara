#include "Gameplay/Swarm/CrowdActor.h"
#include "NiagaraComponent.h"
#include "Gameplay/Player/PlayerCharacter.h"
#include "Gameplay/Swarm/Components/CrowdVisualComponent.h"


ACrowdActor::ACrowdActor() {
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;
	
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	CollisionMesh->SetupAttachment(SphereMesh);
	
	CollisionMesh->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionMesh->SetGenerateOverlapEvents(false);
	CollisionMesh->SetMobility(EComponentMobility::Movable);
	
	NiagaraSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraSystem"));
	NiagaraSystem->SetupAttachment(SphereMesh);
	NiagaraSystem->SetAutoActivate(false);
	
	VisualComp = CreateDefaultSubobject<UCrowdVisualComponent>(TEXT("VisualComp"));
	TargetLocation = FVector(0,0,0);
}


void ACrowdActor::BeginPlay() {
	Super::BeginPlay();
	
	VisualComp->Init(NiagaraSystem, CollisionMesh, SphereMesh);
	VisualComp->Initialize(ActionVisualParams, ParticleSpacing);
	
	VisualComp->ApplyFormVisual(SwarmForm, ActionVisualParams.ParticleCount, ParticleSpacing, ActionVisualParams.MeshScale);
	
	VisualComp->EnsureActiveNiagara();
	SetCrowdState(ECrowdState::FollowingPlayer);
}


void ACrowdActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	switch (CrowdState) {
		case ECrowdState::FollowingPlayer: TickFollowingPlayer(DeltaTime); break;
		case ECrowdState::MovingToTarget:  TickMovingToTarget(DeltaTime);  break;
		case ECrowdState::SlowingDown:     TickSlowingDown(DeltaTime);     break;
		case ECrowdState::StaticForm:      TickIdle(DeltaTime);            break;
		default: break;
	}
}


void ACrowdActor::TickIdle(float) { /* empty */ }


void ACrowdActor::TickFollowingPlayer(float DeltaTime) {
	if (!TargetActor) return;

	Destination = TargetActor->GetActorLocation() + FollowOffset;
	MoveTowardsDestination(DeltaTime);
}


void ACrowdActor::TickMovingToTarget(float DeltaTime) {
	Destination = TargetLocation;
	MoveTowardsDestination(DeltaTime);

	const float Distance = FVector::Dist(GetActorLocation(), Destination);
	if (Distance <= 5.0f && CrowdState == ECrowdState::MovingToTarget) {
		OnDestinationReached.Broadcast();
		SetCrowdState(ECrowdState::SlowingDown);
	}
	VisualComp->ApplyVisualState(ActionVisualParams);
	VisualComp->UpdateBlend(CurrentBlendAlpha);
}


void ACrowdActor::TickSlowingDown(float DeltaTime) {
	VisualComp->UpdateSlowingDown(DeltaTime, CurrentBlendAlpha, ActionVisualParams.BlendAlphaTarget);
	
	if (CurrentBlendAlpha >= ActionVisualParams.BlendAlphaTarget) SetCrowdState(ECrowdState::StaticForm);
}


void ACrowdActor::TickConsumingPlayer(float){ /* todo */ }
void ACrowdActor::TickReforming(float) { /* todo */ }


void ACrowdActor::MoveTowardsDestination(float DeltaTime) {
	FVector NewLocation = FMath::VInterpTo(GetActorLocation(), Destination, DeltaTime, FollowSpeed);
	SetActorLocation(NewLocation);
}


void ACrowdActor::ConsumePlayer(APlayerCharacter* Player) {
	if (!Player) return;

	CrowdState = ECrowdState::ConsumingPlayer;
	
	//PlayConsumeFX();
	// end → OnConsumeFXFinished()
}


void ACrowdActor::OnConsumeFXFinished() {
	OnConsumeFinished.Broadcast();
}


void ACrowdActor::OnPlayerRespawn(AActor* NewPlayer) {
	SetActorLocation(NewPlayer->GetActorLocation());
	TargetActor = NewPlayer;

	//PlayReformFX();

	AttachToActor(NewPlayer, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}


void ACrowdActor::MoveTo(const FVector& NewTargetLocation) {
	TargetActor = nullptr;
	TargetLocation = NewTargetLocation;
	CrowdState = ECrowdState::MovingToTarget;
}


void ACrowdActor::ReturnToPlayer(APlayerCharacter* Player) {
	TargetActor = Player;
	CurrentBlendAlpha = 0.f;
	SetCrowdState(ECrowdState::FollowingPlayer);
}


void ACrowdActor::OnEnterState(ECrowdState NewState) {
	switch (NewState) {
		case ECrowdState::FollowingPlayer:
			CurrentBlendAlpha = 0.f;
			VisualComp->ApplyVisualState(RestVisualParams);
			VisualComp->UpdateBlend(CurrentBlendAlpha);
			break;

		case ECrowdState::MovingToTarget:
		case ECrowdState::SlowingDown:    VisualComp->ApplyVisualState(ActionVisualParams); break;
		case ECrowdState::StaticForm: break;

		case ECrowdState::ConsumingPlayer: break;
		default: break;
	}
}

void ACrowdActor::OnExitState(ECrowdState OldState) {
	switch (OldState) {
		case ECrowdState::SlowingDown: /*VisualComp->DisableCollision(); */  break;
		case ECrowdState::ConsumingPlayer: /*VisualComp->StopConsumeFX(); */ break;
		default: break;
	}
}


void ACrowdActor::SetSwarmForm(ESwarmForm NewSwarmForm){
	if (SwarmForm == NewSwarmForm) return;
	SwarmForm = NewSwarmForm;
	
	VisualComp->ApplyFormVisual(SwarmForm, ActionVisualParams.ParticleCount, ParticleSpacing, ActionVisualParams.MeshScale);
}


void ACrowdActor::SetTargetActor(AActor* NewTarget) {
	TargetActor = NewTarget;
	
	if (TargetActor) {
		SetActorLocation(TargetActor->GetActorLocation() + FollowOffset);
		VisualComp->EnsureActiveNiagara();
	}
}

void ACrowdActor::SetCrowdState(ECrowdState NewState) {
	if (CrowdState == NewState) return;

	OnExitState(CrowdState);
	CrowdState = NewState;
	OnEnterState(NewState);
}


void ACrowdActor::SetActionParticleCount(int Delta) {
	ActionVisualParams.ParticleCount += Delta;
	VisualComp->ApplyVisualState(ActionVisualParams);
}
