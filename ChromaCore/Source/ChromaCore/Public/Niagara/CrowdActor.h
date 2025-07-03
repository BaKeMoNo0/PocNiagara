// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowdActor.generated.h"


class APingMarker;
class UNiagaraComponent;

UCLASS()
class CHROMACORE_API ACrowdActor : public AActor {
	GENERATED_BODY()

	FVector Offset;
	FVector TargetLocation;
	bool bShouldMove = false;
	bool bIsSlowingDown = false;
	
	float InitialDragStrength = 10.0f;
	float InitialAttractionStrength = 120.0f;
	FVector InitialGravityStrength = FVector(0.0f, 0.0f, - 980.0f);
	float InitialVortexStrength = 100.0f;
	float InitialNoiseStrength1 = 700.0f;
	float InitialNoiseStrength2 = -1000.0f;
	float InitialSpeedLimit = 1000.0f;
	float CurrentSpeedLimit = InitialSpeedLimit;

	
public:
	ACrowdActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	float FollowSpeed = 2.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SphereMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraSystem;

	UPROPERTY()
	APingMarker* PingMarker = nullptr;

public:

	UNiagaraComponent* GetNiagaraSystem();
	AActor* GetTargetActor();
	
	void MoveTo(const FVector& NewTargetLocation);
	void SetAllStrengthToZero();
	void SetAllStrengthToInitialValue();
	

	/*
	float GetIniatialDragStrength() const;
	float GetInitialAttractionStrength() const;
	FVector GetInitialGravityStrength() const;
	float GetInitialVortexStrength() const;
	float GetInitialNoiseStrength1() const;
	float GetInitialNoiseStrength2() const;
	float GetInitialSpeedLimit() const;
	*/

	void SetTargetActor(AActor* NewTarget);
	void SetPingMarker(APingMarker* NewPingMarker);



};
