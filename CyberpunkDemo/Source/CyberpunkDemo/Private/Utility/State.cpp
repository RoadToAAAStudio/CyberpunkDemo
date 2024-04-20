// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/State.h"

void UState::EnterState()
{
	UE_LOG(LogTemp, Display, TEXT("Entering state: %s"), *this->GetName());
}

void UState::Tick()
{
}

void UState::ExitState()
{
	UE_LOG(LogTemp, Display, TEXT("Exiting state: %s"), *this->GetName());
}
