// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerPingComponent.generated.h"


class APingMarker;

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

	UPROPERTY(EditAnywhere, Category = "Ping")
	float MaxPingDistance = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Ping")
	float MinPingDistance = 100.f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentPingDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Ping")
	TSubclassOf<APingMarker> PingMarkerClass;

	UPROPERTY()
	APingMarker* ActivePingMarker = nullptr;

public:
	void StartAiming();
	void StopAiming();
	void CallPing();
	void AdjustPingDistance(float AxisValue);

};
