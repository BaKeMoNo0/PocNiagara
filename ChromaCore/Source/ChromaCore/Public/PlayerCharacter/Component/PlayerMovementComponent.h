// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovementComponent.generated.h"


class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHROMACORE_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	APlayerCharacter *OwnerCharacter;

protected:
	virtual void BeginPlay() override;

public:
	void CallMove(const FVector2D MovementVector);
	void CallLook(const FVector2D LookDirection) const;
	void CallRun();
	void CallStopRunning();
	void CallJump() const;
};
