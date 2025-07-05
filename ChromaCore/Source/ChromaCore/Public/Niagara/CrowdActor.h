// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowdActor.generated.h"


class UPlayerPingComponent;
class APlayerCharacter;
class APingMarker;
class UNiagaraComponent;

UCLASS()
class CHROMACORE_API ACrowdActor : public AActor {
	GENERATED_BODY()

	FVector Offset;
	FVector TargetLocation;
	FVector Destination;
	bool bShouldMove = false;
	bool bIsSlowingDown = false;
	bool bHasReachedDestination = false;

	float CurrentBlendAlpha = 0.0f;
	float BlendAlphaTarget = 0.9f;

	
public:
	ACrowdActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Follow")
	AActor* TargetActor;

	UPROPERTY()
	UPlayerPingComponent* PingComp;

	UPROPERTY(VisibleAnywhere, Category = "Follow")
	float FollowSpeed = 2.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SphereMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraSystem;

public:
	void MoveTo(const FVector& NewTargetLocation);
	void ReturnToPlayer(APlayerCharacter* Player);
	void UpdateDestination();
	void MoveTowardsDestination(float DeltaTime);
	void UpdateNiagaraBlending(float DeltaTime);

	UNiagaraComponent* GetNiagaraSystem();
	AActor* GetTargetActor();

	void SetPingComp(UPlayerPingComponent* PingCompRef);
	void SetTargetActor(AActor* NewTarget);
};