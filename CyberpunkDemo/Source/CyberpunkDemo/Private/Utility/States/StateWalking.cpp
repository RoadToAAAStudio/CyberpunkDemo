// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateWalking.h"

void UStateWalking::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(ECustomMovementState::Walking);
	Owner->MaxWalkSpeed = Owner->Walk_MaxWalkSpeed;
}

void UStateWalking::ExitState()
{
	Super::ExitState();
	Owner->SetLastMovementState(ECustomMovementState::Walking);
}

void UStateWalking::Tick()
{
	Super::Tick();
	
}

void UStateWalking::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}
