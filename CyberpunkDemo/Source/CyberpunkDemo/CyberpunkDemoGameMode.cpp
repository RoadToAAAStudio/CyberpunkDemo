// Copyright Epic Games, Inc. All Rights Reserved.

#include "CyberpunkDemoGameMode.h"
#include "CyberpunkDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACyberpunkDemoGameMode::ACyberpunkDemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
