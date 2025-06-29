// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter/Component/PlayerSoundComponent.h"

#include "Components/AudioComponent.h"
#include "PlayerCharacter/PlayerCharacter.h"


UPlayerSoundComponent::UPlayerSoundComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerSoundComponent::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerSoundComponent::CallSound() {
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player && Player->GetAudioComponent()) Player->GetAudioComponent()->Play();
}



