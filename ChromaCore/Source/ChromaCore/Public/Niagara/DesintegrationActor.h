#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "DesintegrationActor.generated.h"

class APlayerCharacter;
class UNiagaraComponent;

UCLASS()
class CHROMACORE_API ADesintegrationActor : public AActor {
	GENERATED_BODY()

	bool bPlayerIsNear = false;
	
public:
	ADesintegrationActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* SphereComp;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter = nullptr;

	UFUNCTION()
	void OnPlayerEnterRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							 bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerExitRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:
	virtual void Tick(float DeltaTime) override;

	bool GetPlayerIsNear() const;
	UNiagaraComponent* GetNiagaraComp() const;
	UStaticMeshComponent* GetMesh() const;
	USphereComponent* GetSphereCollision() const;
	APlayerCharacter* GetPlayerCharacter() const;
};
