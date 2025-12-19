#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Footstep.generated.h"

UCLASS()
class CHROMACORE_API AFootstep : public AActor {
	GENERATED_BODY()
	
public:	
	AFootstep();

protected:
	virtual void BeginPlay() override;

};
