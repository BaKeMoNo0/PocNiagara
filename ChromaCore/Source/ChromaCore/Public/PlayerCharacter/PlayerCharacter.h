// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Niagara/CrowdActor.h"
#include "PlayerCharacter.generated.h"

class UPlayerSoundComponent;
class UPlayerPingComponent;
class UPlayerMovementComponent;

UCLASS()
class CHROMACORE_API APlayerCharacter : public ACharacter {
	GENERATED_BODY()

public:
	APlayerCharacter();

	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent *SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent *CameraComp;

	UPROPERTY()
	UPlayerMovementComponent *PlayerMovementComponent;
	UPROPERTY()
	UPlayerPingComponent *PlayerPingComponent;
	UPROPERTY()
	UPlayerSoundComponent *PlayerSoundComponent;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<ACrowdActor> CrowdActorClass;
	UPROPERTY()
	ACrowdActor* SpawnedCrowdActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* AudioComponent;
	
public:
	UPlayerMovementComponent *GetPlayerMovementComponent() const;
	UPlayerPingComponent *GetPlayerPingComponent() const;
	UPlayerSoundComponent *GetPlayerSoundComponent() const;
	
	UAudioComponent *GetAudioComponent() const;
	ACrowdActor *GetCrowdActor() const;
};
