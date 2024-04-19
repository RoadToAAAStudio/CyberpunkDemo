// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateRunning.h"

void UStateRunning::EnterState()
{
	Super::EnterState();
	Owner->MaxWalkSpeed = Owner->Sprint_MaxWalkSpeed;
	//Owner->bWantsToCrouchCustom = false;
}

void UStateRunning::ExitState()
{
	Super::ExitState();
}

void UStateRunning::Tick()
{
	Super::Tick();
}
