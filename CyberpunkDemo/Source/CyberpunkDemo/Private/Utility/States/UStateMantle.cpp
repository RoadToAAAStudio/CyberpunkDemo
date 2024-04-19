// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/UStateMantle.h"
#include "MainCharacter/MainCharacter.h"

void UUStateMantle::EnterState()
{
	Super::EnterState();
	// Reset the jump boolean
	Owner->bWantsToJump = false;
	Owner->bWantsToRun = false;
	// Set the player velocity to 0
	Owner->Velocity = FVector(0,0,0);
	Owner->MainCharacter->DisableInput(Cast<APlayerController>(Owner->MainCharacter->GetController()));
	Owner->MainCharacter->Mantle();
}

void UUStateMantle::ExitState()
{
	Super::ExitState();
	Owner->MainCharacter->EnableInput(Cast<APlayerController>(Owner->MainCharacter->GetController()));
}

void UUStateMantle::Tick()
{
	Super::Tick();
}
