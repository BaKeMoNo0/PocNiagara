#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwarmAbilityComponent.generated.h"


class ASwarmPlatform;
class APlayerCharacter;
class ASwarmPlaneActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSWARM_API USwarmAbilityComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	USwarmAbilityComponent();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<ASwarmPlaneActor> SwarmPlaneClass;
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<ASwarmPlatform> FootstepActorClass;
	UPROPERTY()
	ASwarmPlatform* SpawnedFootstepActor;
	
	UPROPERTY()
	bool bExpectLeftFoot = true;
	
	UPROPERTY(EditAnywhere, Category = "Footstep")
	float SideOffset = 25.f;
	
	UPROPERTY(EditAnywhere, Category = "Footstep")
	float ForwardOffset = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Footstep")
	float StepImpulse = 500.f;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage *AnimStrideFootL;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage *AnimStrideFootR;
	
public:
	void TrySpawnPlane(APlayerCharacter* Player);
	void TrySpawnFootStep(APlayerCharacter* Player, bool bIsLeftFoot);
	
};
