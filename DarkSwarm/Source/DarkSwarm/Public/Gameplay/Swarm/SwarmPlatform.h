#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwarmPlatform.generated.h"

UCLASS()
class DARKSWARM_API ASwarmPlatform : public AActor {
	GENERATED_BODY()
	
public:	
	ASwarmPlatform();

protected:
	virtual void BeginPlay() override;

};
