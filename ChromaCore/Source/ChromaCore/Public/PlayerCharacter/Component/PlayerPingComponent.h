// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerPingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHROMACORE_API UPlayerPingComponent : public UActorComponent {
	GENERATED_BODY()
	
	bool bIsAiming = false;
	FVector LastValidLocation;

	void TraceFromCrosshair();

public:
	UPlayerPingComponent();

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void StartAiming();
	void StopAiming();
	void CallPing();

};
