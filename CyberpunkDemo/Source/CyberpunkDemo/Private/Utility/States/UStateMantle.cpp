// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/UStateMantle.h"

#include "MainCharacter/MainCharacter.h"

class UEnhancedInputLocalPlayerSubsystem;

void UUStateMantle::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UUStateMantle::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(ECustomMovementState::Mantling);
	// Reset the jump boolean
	Owner->bWantsToJump = false;
	// Set the player velocity to 0
	Owner->Velocity = FVector(0,0,0);
	Owner->MainCharacter->DisableMappingContext(true);
	Owner->MainCharacter->Mantle();
}

void UUStateMantle::ExitState()
{
	Super::ExitState();
	Owner->MainCharacter->DisableMappingContext(false);
}

void UUStateMantle::Tick()
{
	Super::Tick();
}
