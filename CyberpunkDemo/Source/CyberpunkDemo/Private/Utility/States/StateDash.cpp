// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateDash.h"

#include "MainCharacter/CustomPlayerController.h"
#include "MainCharacter/MainCharacter.h"

void UStateDash::EnterState()
{
	Super::EnterState();
	Owner->MaxWalkSpeed = Owner->DashSpeedMultiplier * Owner->Walk_MaxWalkSpeed;
	Owner->GetWorld()->GetTimerManager().SetTimer(DashTimer, FTimerDelegate::CreateLambda([this] { Owner->bWantsToDash = false; }), Owner->DashDuration, false);
}

void UStateDash::Tick()
{
	Super::Tick();
}

void UStateDash::ExitState()
{
	Super::ExitState();
}
