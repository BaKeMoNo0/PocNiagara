// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrowdActor.generated.h"


class UNiagaraComponent;

UCLASS()
class CHROMACORE_API ACrowdActor : public AActor {
	GENERATED_BODY()

	FVector Offset;
	FVector TargetLocation;
	bool bShouldMove = false;
	
public:
	ACrowdActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	AActor* TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	float FollowSpeed = 5.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SphereMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNiagaraComponent* NiagaraSystem;

public:
	void MoveTo(const FVector& NewTargetLocation);

	AActor* GetTargetActor();
	void SetTargetActor(AActor* NewTarget);
};
