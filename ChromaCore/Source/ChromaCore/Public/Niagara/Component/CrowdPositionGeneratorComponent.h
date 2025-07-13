#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/ActorComponent.h"
#include "CrowdPositionGeneratorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHROMACORE_API UCrowdPositionGeneratorComponent : public UActorComponent {
	GENERATED_BODY()

protected:
	UPROPERTY()
	TArray<FVector> GeneratedPositions;

public:	
	void GeneratePositions(int32 SpawnCount, float Spacing, int32 FormType);
	void PushToNiagara(UNiagaraComponent* Niagara);
};
