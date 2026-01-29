#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Gameplay/Swarm/Types/SwarmForm.h"
#include "CrowdVisualComponent.generated.h"


/**
 * Pure visual/physical representation of the swarm.
 * NO gameplay logic.
 */

struct FCrowdVisualParams;
class UNiagaraComponent;
class UStaticMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSWARM_API UCrowdVisualComponent : public UActorComponent {
	GENERATED_BODY()

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
};
