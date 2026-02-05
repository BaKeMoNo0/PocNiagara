#include "Gameplay/Player/PlayerCharacter.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Core/DarkSwarmGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Player/Components/PlayerInteractionComponent.h"
#include "Gameplay/Player/Components/PlayerMovementComponent.h"
#include "Gameplay/Player/Components/PlayerPingComponent.h"
#include "Gameplay/Player/Components/PlayerSoundComponent.h"
#include "Gameplay/Player/Components/SwarmAbilityComponent.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 250.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	PlayerMovementComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("PlayerMovementComp"));
	PlayerPingComponent = CreateDefaultSubobject<UPlayerPingComponent>(TEXT("PlayerPingComp"));
	PlayerSoundComponent = CreateDefaultSubobject<UPlayerSoundComponent>(TEXT("PlayerSoundComp"));
	PlayerInteractionComponent = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("PlayerInteractionComp"));
	SwarmAbilityComponent = CreateDefaultSubobject<USwarmAbilityComponent>(TEXT("SwarmAbilityComp"));
	
}


void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();
	
	LastCheckpointLocation = GetActorLocation();
}



void APlayerCharacter::Die() {
	if (bIsDead) return;

	bIsDead = true;
	
	GetCharacterMovement()->DisableMovement();
	SetActorEnableCollision(false);
	
	if (ADarkSwarmGameMode* GM = Cast<ADarkSwarmGameMode>(GetWorld()->GetAuthGameMode())) GM->OnPlayerDied(this);
}
