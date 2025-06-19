// Copyright Epic Games, Inc. All Rights Reserved.

#include "ChromaCoreGameMode.h"
#include "ChromaCoreCharacter.h"
#include "UObject/ConstructorHelpers.h"

AChromaCoreGameMode::AChromaCoreGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
