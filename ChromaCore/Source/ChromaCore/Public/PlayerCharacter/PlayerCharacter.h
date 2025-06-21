// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

class UPlayerMovementComponent;

UCLASS()
class CHROMACORE_API APlayerCharacter : public ACharacter {
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent *SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent *CameraComp;

	UPROPERTY()
	UPlayerMovementComponent *PlayerMovementComponent;
	
protected:
	virtual void BeginPlay() override;

public:
	UPlayerMovementComponent *GetPlayerMovementComponent() const;
};
