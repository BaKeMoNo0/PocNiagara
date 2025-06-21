// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter/Component/PlayerMovementComponent.h"

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

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate              = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane         = false;
	GetCharacterMovement()->bSnapToPlaneAtStart       = false;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//PrimaryActorTick.bCanEverTick          = true;
	//PrimaryActorTick.bStartWithTickEnabled = true;
}


void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();
	
	PlayerMovementComponent = FindComponentByClass<UPlayerMovementComponent>();
}

UPlayerMovementComponent* APlayerCharacter::GetPlayerMovementComponent() const{ return PlayerMovementComponent;}

