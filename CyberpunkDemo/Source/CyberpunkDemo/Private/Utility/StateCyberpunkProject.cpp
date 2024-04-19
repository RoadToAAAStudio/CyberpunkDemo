// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/StateCyberpunkProject.h"

void UStateCyberpunkProject::Initialize(TObjectPtr<UCustomCharacterMovementComponent> owner, ECustomMovementState state)
{
	Owner = owner;
	State = state;
}

void UStateCyberpunkProject::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(State);
	Owner->OnEnterCustomMovementState.Broadcast(State);
}

void UStateCyberpunkProject::Tick()
{
	Super::Tick();
}

void UStateCyberpunkProject::ExitState()
{
	Super::ExitState();
	Owner->SetLastMovementState(State);
	Owner->OnExitCustomMovementState.Broadcast(State);
}
