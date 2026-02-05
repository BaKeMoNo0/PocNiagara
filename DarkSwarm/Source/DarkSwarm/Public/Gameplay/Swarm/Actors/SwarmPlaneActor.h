#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwarmPlaneActor.generated.h"

UCLASS()
class DARKSWARM_API ASwarmPlaneActor : public AActor {
	GENERATED_BODY()
	
public:
	ASwarmPlaneActor();

	void Activate(float Lifetime);
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PlaneMesh;

	void Expire();

};
