// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateIdle.h"


void UStateIdle::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UStateIdle::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(ECustomMovementState::Idle);
}

void UStateIdle::ExitState()
{
	Super::ExitState();
}

void UStateIdle::Tick()
{
	Super::Tick();
}
