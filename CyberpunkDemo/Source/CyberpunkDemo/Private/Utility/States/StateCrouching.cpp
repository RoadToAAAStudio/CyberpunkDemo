// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateCrouching.h"

#include "Camera/CameraComponent.h"
#include "MainCharacter/MainCharacter.h"

void UStateCrouching::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UStateCrouching::EnterState()
{
	Super::EnterState();
	Owner->SetCurrentMovementState(ECustomMovementState::Crouching);
	Owner->MaxWalkSpeedCrouched = Owner->Crouch_MaxWalkSpeed;
	Owner->MainCharacter->Crouch();
}

void UStateCrouching::ExitState()
{
	Super::ExitState();
	Owner->bWantsToCrouchCustom = false;
	Owner->MainCharacter->UnCrouch();
	Owner->SetLastMovementState(ECustomMovementState::Crouching);
}

void UStateCrouching::Tick()
{
	Super::Tick();
	
}
