// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateMantle.h"
#include "MainCharacter/MainCharacter.h"

void UStateMantle::EnterState()
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

void UStateMantle::ExitState()
{
	Super::ExitState();
	Owner->MainCharacter->EnableInput(Cast<APlayerController>(Owner->MainCharacter->GetController()));
}

void UStateMantle::Tick()
{
	Super::Tick();
}
