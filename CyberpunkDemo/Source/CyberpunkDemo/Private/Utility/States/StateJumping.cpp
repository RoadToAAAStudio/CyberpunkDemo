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
	Owner->MainCharacter->Jump();
}

void UStateJumping::ExitState()
{
	Super::ExitState();

	Owner->MainCharacter->StopJumping();
}

void UStateJumping::Tick()
{
	Super::Tick();
}
