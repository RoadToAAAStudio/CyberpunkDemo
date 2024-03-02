// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateJumping.h"

#include "MainCharacter/MainCharacter.h"

void UStateJumping::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UStateJumping::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(ECustomMovementState::Jumping);
	// if (Owner->MainCharacter->bIsCrouched) Owner->MainCharacter->bIsCrouched = false;
	//
	// Owner->MainCharacter->Jump();
	bHasJumped = false;
}

void UStateJumping::ExitState()
{
	Super::ExitState();

	Owner->MainCharacter->StopJumping();
}

void UStateJumping::Tick()
{
	Super::Tick();
	if (!Owner->MainCharacter->bIsCrouched && !bHasJumped)
	{
		bHasJumped = true;
		Owner->MainCharacter->Jump();
	}

	// if (Owner->Velocity.Z < 0)
	// {
	// 	Owner->TryMantle();
	// }
}
