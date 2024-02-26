// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateWalking.h"

void UStateWalking::EnterState()
{
	Super::EnterState();
	Owner->MaxWalkSpeed = Owner->Walk_MaxWalkSpeed;
	Owner->SetCurrentMovementState(ECustomMovementState::Walking);
}

void UStateWalking::ExitState()
{
	Super::ExitState();
}

void UStateWalking::Tick()
{
	Super::Tick();
	
}

void UStateWalking::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}
