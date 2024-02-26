// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/FState.h"

void UFState::EnterState()
{
	UE_LOG(LogTemp, Display, TEXT("Entering state: %s"), *this->GetName());
}

void UFState::Tick()
{
}

void UFState::ExitState()
{
	UE_LOG(LogTemp, Display, TEXT("Exiting state: %s"), *this->GetName());
}
