// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/Swarm/Types/SwarmForm.h"
#include "PingMarker.generated.h"

class UWidgetComponent;

UCLASS()
class DARKSWARM_API APingMarker : public AActor {
	GENERATED_BODY()
	
public:
	APingMarker();

protected:
	virtual void BeginPlay();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* VisualMesh;
	
	UPROPERTY(EditAnywhere, Category = "Ping|Material")
	UMaterialInterface* PingMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ping|Mesh")
	UStaticMesh* CubeMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Ping|Mesh")
	UStaticMesh* PlaneMesh;




public:
	void SetFormVisual(ESwarmForm Type);
};
