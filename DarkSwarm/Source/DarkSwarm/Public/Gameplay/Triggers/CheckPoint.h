#pragma once

#include "CoreMinimal.h"
#include "BaseTriggerBox.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

UCLASS()
class DARKSWARM_API ACheckPoint : public ABaseTriggerBox {
	GENERATED_BODY()
	
public:	
	ACheckPoint();

protected:
	virtual void BeginPlay() override;
	virtual void OnPlayerTriggered(APlayerCharacter* Player) override;
	
	UPROPERTY(VisibleAnywhere, Category="Checkpoint")
	bool bIsActivated = false;
};
