// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Player/PlayerCharacter.h"

#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Swarm/SwarmPlatform.h"
#include "Gameplay/Player/Component/PlayerMovementComponent.h"
#include "Gameplay/Player/Component/PlayerPingComponent.h"
#include "Gameplay/Player/Component/PlayerSoundComponent.h"

APlayerCharacter::APlayerCharacter() {
	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset    = FVector(-120.f, 0.f, 115.f);

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
	//PrimaryActorTick.bCanEverTick          = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
}


void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();
	
	PlayerMovementComponent = FindComponentByClass<UPlayerMovementComponent>();
	PlayerPingComponent = FindComponentByClass<UPlayerPingComponent>();
	PlayerSoundComponent = FindComponentByClass<UPlayerSoundComponent>();
	
	if (!SpawnedCrowdActor && CrowdActorClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = GetActorLocation() + FVector(-100.f, -90.f, 100.f);
		FRotator SpawnRotation = FRotator::ZeroRotator;

		SpawnedCrowdActor = GetWorld()->SpawnActor<ACrowdActor>(CrowdActorClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedCrowdActor) {
			SpawnedCrowdActor->SetTargetActor(this);
			SpawnedCrowdActor->SetPingComp(PlayerPingComponent);
		}
	}

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

UPlayerMovementComponent* APlayerCharacter::GetPlayerMovementComponent() const{ return PlayerMovementComponent;}
UPlayerPingComponent* APlayerCharacter::GetPlayerPingComponent() const { return PlayerPingComponent; }
UPlayerSoundComponent* APlayerCharacter::GetPlayerSoundComponent() const { return PlayerSoundComponent;}
UAudioComponent* APlayerCharacter::GetAudioComponent() const { return AudioComponent;}
ACrowdActor* APlayerCharacter::GetCrowdActor() const { return SpawnedCrowdActor;}


