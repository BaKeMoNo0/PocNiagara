// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/Swarm/CrowdActor.h"
#include "Gameplay/Swarm/SwarmPlatform.h"
#include "PlayerCharacter.generated.h"

class UPlayerSoundComponent;
class UPlayerPingComponent;
class UPlayerMovementComponent;

UCLASS()
class DARKSWARM_API APlayerCharacter : public ACharacter {
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
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<ASwarmPlatform> FootstepActorClass;
	UPROPERTY()
	ASwarmPlatform* SpawnedFootstepActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* AudioComponent;
	
	UPROPERTY(EditAnywhere, Category = "Footstep")
	float SideOffset = 25.f;
	
	UPROPERTY(EditAnywhere, Category = "Footstep")
	float ForwardOffset = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Footstep")
	float StepImpulse = 500.f;
	UPROPERTY()
    bool bExpectLeftFoot = true;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage *AnimStrideFootL;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage *AnimStrideFootR;
	
	//timeBetweenSameFootContacts = strideDistancePerStep / characterVelocity
	//timeBetweenSameFootContacts = (0.85 meters) / (playerSpeed m/s)
	//strideStepDuration = timeBetweenSameFootContacts
	//inputWindow = clamp( strideStepDuration * 0.35 , 150ms , 400ms )
	//inputBuffer = clamp( strideStepDuration * 0.1  ,  50ms , 120ms )
	/*
		OnNotify("RightFoot_Contact"):
		if lastRightFootTime > 0:
			timeBetweenSameFootContacts = currentTime - lastRightFootTime
		lastRightFootTime = currentTime
	 */


public:
	void TrySpawnFootStep(bool bIsLeftFoot);
	
	UPlayerMovementComponent *GetPlayerMovementComponent() const;
	UPlayerPingComponent *GetPlayerPingComponent() const;
	UPlayerSoundComponent *GetPlayerSoundComponent() const;
	
	UAudioComponent *GetAudioComponent() const;
	ACrowdActor *GetCrowdActor() const;
};
