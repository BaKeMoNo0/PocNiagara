#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DisintegratableComponent.generated.h"

class ADarkSwarmGameMode;
class ADesintegrationActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSWARM_API UDisintegratableComponent : public UActorComponent {
	GENERATED_BODY()

	FVector CurrentCrowdLocation;
	bool bIsDisintegrating = false;
	UPROPERTY()
	ADarkSwarmGameMode* GM;

public:
	UDisintegratableComponent();

protected:
	ADesintegrationActor* Owner;
	FTimerHandle FadeHandle;
	UPROPERTY()
	UMaterialInstanceDynamic* DynMat;
	
	
	virtual void BeginPlay() override;
	void FadeOutQuick();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TriggerDisintegration();
};
