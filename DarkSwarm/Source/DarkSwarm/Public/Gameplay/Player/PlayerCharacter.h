// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/Swarm/SwarmPlatform.h"
#include "PlayerCharacter.generated.h"

class UPlayerInteractionComponent;
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
	UPROPERTY()
	UPlayerInteractionComponent *PlayerInteractionComponent;

	
	
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
	
	UPROPERTY()
	FVector LastCheckpointLocation;
	
	UPROPERTY()
	bool bIsDead = false;


public:
	void TrySpawnFootStep(bool bIsLeftFoot);
	void Die();
	
	UPlayerMovementComponent* GetPlayerMovementComponent() const { return PlayerMovementComponent;}
	UPlayerPingComponent* GetPlayerPingComponent() const { return PlayerPingComponent; }
	UPlayerSoundComponent* GetPlayerSoundComponent() const { return PlayerSoundComponent;}
	UPlayerInteractionComponent* GetPlayerInteractionComponent() const { return PlayerInteractionComponent;}
	
	UAudioComponent* GetAudioComponent() const { return AudioComponent;}
	FVector GetLastCheckpointLocation() const { return LastCheckpointLocation; }
	bool GetIsDead() const { return bIsDead; }
	

	void SetLastCheckpointLocation(FVector NewRespawnLocation) { LastCheckpointLocation = NewRespawnLocation; }
};
