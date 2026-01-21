#pragma once

#include "CoreMinimal.h"
#include "BaseTriggerBox.h"
#include "GameFramework/Actor.h"
#include "DeathBox.generated.h"

UCLASS()
class DARKSWARM_API ADeathBox : public ABaseTriggerBox {
	GENERATED_BODY()
	
public:
	ADeathBox();

protected:
	virtual void BeginPlay() override;
	
	virtual void OnPlayerTriggered(APlayerCharacter* Player) override;
};
