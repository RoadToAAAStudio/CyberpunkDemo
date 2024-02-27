// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateCrouching.h"

#include "MainCharacter/MainCharacter.h"

void UStateCrouching::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UStateCrouching::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(ECustomMovementState::Crouching);
	Owner->MainCharacter->Crouch();
}

void UStateCrouching::ExitState()
{
	Super::ExitState();
	Owner->MainCharacter->UnCrouch();
}

void UStateCrouching::Tick()
{
	Super::Tick();
}
