// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "./FormType.h"
#include "PingMarker.generated.h"

class UWidgetComponent;

UCLASS()
class CHROMACORE_API APingMarker : public AActor {
	GENERATED_BODY()
	
public:
	APingMarker();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere, Category = "Ping")
	UMaterialInterface* PingMaterial;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* CustomMesh;

public:
	void SetFormVisual(EFormType Type);
};
