#include "Gameplay/Player/PlayerCharacter.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/DarkSwarmGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Swarm/SwarmPlatform.h"
#include "Gameplay/Player/Components/PlayerMovementComponent.h"
#include "Gameplay/Player/Components/PlayerPingComponent.h"
#include "Gameplay/Player/Components/PlayerSoundComponent.h"

APlayerCharacter::APlayerCharacter() {
	PrimaryActorTick.bCanEverTick = false;
	
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->InitCapsuleSize(34.f, 88.f);
	
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Capsule->SetGenerateOverlapEvents(true); 
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 350.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset    = FVector(-140.f, 0.f, 140.f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate              = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane         = false;
	GetCharacterMovement()->bSnapToPlaneAtStart       = false;

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}


void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();
	
	PlayerMovementComponent = FindComponentByClass<UPlayerMovementComponent>();
	PlayerPingComponent = FindComponentByClass<UPlayerPingComponent>();
	PlayerSoundComponent = FindComponentByClass<UPlayerSoundComponent>();
	LastCheckpointLocation = GetActorLocation();
}


void APlayerCharacter::TrySpawnFootStep(bool bIsLeftFoot) {
	//if (bIsLeftFoot != bExpectLeftFoot) return;

	if (!FootstepActorClass) return;

	const float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector SpawnLocation = GetActorLocation();
	SpawnLocation.Z -= CapsuleHalfHeight + 5.f;

	SpawnLocation += GetActorForwardVector() * ForwardOffset;
	SpawnLocation += GetActorRightVector() * (bIsLeftFoot ? -SideOffset : SideOffset);
	
	if (bIsLeftFoot) 
		PlayAnimMontage(AnimStrideFootL, 0.4);
	else 
		PlayAnimMontage(AnimStrideFootR, 0.4);
	
	LaunchCharacter(
		GetActorForwardVector() * StepImpulse,
		false,
		false
	);
	
	GetWorld()->SpawnActor<ASwarmPlatform>(
		FootstepActorClass,
		SpawnLocation,	
		FRotator::ZeroRotator
	);
	
	bExpectLeftFoot = !bExpectLeftFoot;
}

void APlayerCharacter::Die() {
	if (bIsDead) return;

	bIsDead = true;
	
	GetCharacterMovement()->DisableMovement();
	SetActorEnableCollision(false);
	
	if (ADarkSwarmGameMode* GM = Cast<ADarkSwarmGameMode>(GetWorld()->GetAuthGameMode())) GM->OnPlayerDied(this);
}
