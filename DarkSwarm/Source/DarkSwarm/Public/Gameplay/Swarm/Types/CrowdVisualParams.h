#pragma once

#include "CoreMinimal.h"
#include "CrowdVisualParams.generated.h"

USTRUCT(BlueprintType)
struct FCrowdVisualParams {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 ParticleCount = 50;

	UPROPERTY(EditAnywhere)
	float MeshScale = 0.07f;

	UPROPERTY(EditAnywhere)
	float BlendAlphaTarget = 0.9f;
	
	UPROPERTY(EditAnywhere)
	bool bEnableCollision = false;
};

