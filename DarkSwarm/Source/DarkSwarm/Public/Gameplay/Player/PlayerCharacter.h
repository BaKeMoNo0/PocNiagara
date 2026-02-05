// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/Swarm/SwarmPlatform.h"
#include "PlayerCharacter.generated.h"

class USwarmAbilityComponent;
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

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPlayerMovementComponent *PlayerMovementComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPlayerPingComponent *PlayerPingComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPlayerSoundComponent *PlayerSoundComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UPlayerInteractionComponent *PlayerInteractionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USwarmAbilityComponent *SwarmAbilityComponent;

	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* AudioComponent;
	
	UPROPERTY()
	FVector LastCheckpointLocation;
	
	UPROPERTY()
	bool bIsDead = false;
	
public:
	void Die();
	
	UPlayerMovementComponent* GetPlayerMovementComponent() const { return PlayerMovementComponent;}
	UPlayerPingComponent* GetPlayerPingComponent() const { return PlayerPingComponent; }
	UPlayerSoundComponent* GetPlayerSoundComponent() const { return PlayerSoundComponent;}
	UPlayerInteractionComponent* GetPlayerInteractionComponent() const { return PlayerInteractionComponent;}
	USwarmAbilityComponent* GetSwarmAbilityComponent() const { return SwarmAbilityComponent;}
	
	UAudioComponent* GetAudioComponent() const { return AudioComponent;}
	FVector GetLastCheckpointLocation() const { return LastCheckpointLocation; }
	bool GetIsDead() const { return bIsDead; }
	

	void SetLastCheckpointLocation(FVector NewRespawnLocation) { LastCheckpointLocation = NewRespawnLocation; }
};
