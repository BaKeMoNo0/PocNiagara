#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/Swarm/Types/SwarmForm.h"
#include "CrowdVisualComponent.generated.h"


/**
 * Handles purely visual feedback for the swarm.
 * No gameplay state changes should be performed here directly.
 */


class ADisintegratableActor;
struct FCrowdVisualParams;
class UNiagaraComponent;
class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSWARM_API UCrowdVisualComponent : public UActorComponent {
	GENERATED_BODY()
	
protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(
		FOnAbsorptionFinished,
		ADisintegratableActor*
		);
	FOnAbsorptionFinished OnAbsorptionFinished;

public:
	void Init(UNiagaraComponent* InNiagara, UStaticMeshComponent* InCollisionMesh, UStaticMeshComponent* InSphere);
	void Initialize(const FCrowdVisualParams& MaxVisualParams, float ParticleSpacing);
	
	// Called ONCE when entering a visual state
	void ApplyVisualState(const FCrowdVisualParams& VisualParams);
	// Called EVERY FRAME when blending
	void UpdateBlend(float BlendAlpha); 
	
	void UpdateSlowingDown(float DeltaTime, float& InOutBlendAlpha, float BlendTarget);
	void ApplyFormVisual(ESwarmForm SwarmForm, int ParticleCount, float Spacing, float MeshScale);
	void EnsureActiveNiagara();
	
	/**
	 * Starts the visual absorption of a disintegrated actor.
	 * Completion is reported via OnAbsorptionFinished.
	 */
	void BeginAbsorption(ADisintegratableActor* DisActor);
	void UpdateAttractionTarget(UNiagaraComponent* Niagara);
	
	FOnAbsorptionFinished& OnAbsorptionFinishedEvent() { return OnAbsorptionFinished; }
	
private:
	UPROPERTY()
	UNiagaraComponent* Niagara;

	UPROPERTY()
	UStaticMeshComponent* CollisionMesh;

	UPROPERTY()
	USceneComponent* Sphere;
	
	UPROPERTY(EditDefaultsOnly, Category="Swarm|Mesh")
	UStaticMesh* CubeMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Swarm|Mesh")
	UStaticMesh* PlaneMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Swarm|Mesh")
	UMaterialInterface* InvisibleMaterial;
	
	UPROPERTY()
	ADisintegratableActor* CurrentAbsorbingActor;
	
protected:
	UFUNCTION()
	void HandleNiagaraFinished(UNiagaraComponent* FinishedComponent);
	
};
