// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateCrouching.h"
#include "MainCharacter/MainCharacter.h"

void UStateCrouching::EnterState()
{
	Super::EnterState();
	Owner->MaxWalkSpeedCrouched = Owner->Crouch_MaxWalkSpeed;
	Owner->MainCharacter->Crouch();
}

void UStateCrouching::ExitState()
{
	Super::ExitState();
	Owner->bWantsToCrouchCustom = false;
	Owner->MainCharacter->UnCrouch();
}

void UStateCrouching::Tick()
{
	Super::Tick();
}
