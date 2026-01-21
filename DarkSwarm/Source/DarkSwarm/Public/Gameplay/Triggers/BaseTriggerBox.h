#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Gameplay/Player/PlayerCharacter.h"
#include "BaseTriggerBox.generated.h"

UCLASS(Abstract)
class DARKSWARM_API ABaseTriggerBox : public AActor {
	GENERATED_BODY()
	
public:	
	
	ABaseTriggerBox();

protected:
	virtual void BeginPlay() override;
	virtual void OnPlayerTriggered(APlayerCharacter* Player);
	
	UPROPERTY()
	UBoxComponent* BoxComponent = nullptr;
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
