// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/StateVault.h"
#include "MainCharacter/MainCharacter.h"

void UStateVault::EnterState()
{
	Super::EnterState();
	Owner->MainCharacter->DisableInput(Cast<APlayerController>(Owner->MainCharacter->GetController()));
	Owner->bWantsToJump = false;
	Owner->MainCharacter->Vault();
}

void UStateVault::ExitState()
{
	Super::ExitState();
	Owner->MainCharacter->EnableInput(Cast<APlayerController>(Owner->MainCharacter->GetController()));
}

void UStateVault::Tick()
{
	Super::Tick();
}
