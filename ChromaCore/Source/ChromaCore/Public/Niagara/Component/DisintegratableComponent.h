#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DisintegratableComponent.generated.h"

class ADesintegrationActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHROMACORE_API UDisintegratableComponent : public UActorComponent
{
	GENERATED_BODY()

	ADesintegrationActor* Owner;

public:
	UDisintegratableComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TriggerDisintegration();
};
