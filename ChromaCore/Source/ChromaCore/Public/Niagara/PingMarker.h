// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PingMarker.generated.h"

class UWidgetComponent;

UCLASS()
class CHROMACORE_API APingMarker : public AActor {
	GENERATED_BODY()
	
public:
	APingMarker();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* MarkerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ping")
	float Lifetime = 2.f;

	FTimerHandle LifetimeTimer;
};
