// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/States/UStateMantle.h"

#include "MainCharacter/MainCharacter.h"

void UUStateMantle::SetOwner(TObjectPtr<UCustomCharacterMovementComponent> owner)
{
	Owner = owner;
}

void UUStateMantle::EnterState()
{
	Super::EnterState();
	Owner->MainCharacter->Mantle();
}

void UUStateMantle::ExitState()
{
	Super::ExitState();
}

void UUStateMantle::Tick()
{
	Super::Tick();
}
