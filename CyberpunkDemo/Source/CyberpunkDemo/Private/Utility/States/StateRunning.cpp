// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateRunning.h"

void UStateRunning::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UStateRunning::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(ECustomMovementState::Running);
	Owner->MaxWalkSpeed = Owner->Sprint_MaxWalkSpeed;
	//Owner->bWantsToCrouchCustom = false;
}

void UStateRunning::ExitState()
{
	Super::ExitState();
	Owner->SetLastMovementState(ECustomMovementState::Running);
}

void UStateRunning::Tick()
{
	Super::Tick();
}
