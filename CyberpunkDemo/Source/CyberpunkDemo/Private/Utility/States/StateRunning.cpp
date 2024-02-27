// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateRunning.h"

void UStateRunning::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UStateRunning::EnterState()
{
	Super::EnterState();
	Owner->MaxWalkSpeed = Owner->Sprint_MaxWalkSpeed;
	Owner->bWantsToCrouchCustom = false;
	Owner->SetCurrentMovementState(ECustomMovementState::Running);
}

void UStateRunning::ExitState()
{
	Super::ExitState();
}

void UStateRunning::Tick()
{
	Super::Tick();
}
