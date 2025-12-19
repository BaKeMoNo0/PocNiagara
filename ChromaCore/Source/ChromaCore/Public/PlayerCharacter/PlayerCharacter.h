// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Niagara/CrowdActor.h"
#include "Niagara/Footstep.h"
#include "PlayerCharacter.generated.h"

class UPlayerSoundComponent;
class UPlayerPingComponent;
class UPlayerMovementComponent;

UCLASS()
class CHROMACORE_API APlayerCharacter : public ACharacter {
	GENERATED_BODY()
	
	bool bLeftStepNext = false;

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
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<AFootstep> FootstepActorClass;
	UPROPERTY()
	AFootstep* SpawnedFootstepActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* AudioComponent;
	
	UPROPERTY(EditAnywhere, Category = "Footstep")
	float StepDistance = 150.f;

	UPROPERTY(EditAnywhere, Category = "Footstep")
	float FootOffset = 90.f;

	UPROPERTY(EditAnywhere, Category = "Footstep")
	float SideOffset = 25.f;
	
	UPROPERTY(EditAnywhere, Category = "Footstep")
	float ForwardOffset = 25.f;

	
public:
	void TrySpawnFootStep();
	
	UPlayerMovementComponent *GetPlayerMovementComponent() const;
	UPlayerPingComponent *GetPlayerPingComponent() const;
	UPlayerSoundComponent *GetPlayerSoundComponent() const;
	
	UAudioComponent *GetAudioComponent() const;
	ACrowdActor *GetCrowdActor() const;
	
	bool GetLeftStepNext() const;
	void SetLeftStepNext(bool value);

};
