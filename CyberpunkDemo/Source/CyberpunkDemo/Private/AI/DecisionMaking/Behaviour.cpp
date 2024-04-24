// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/DecisionMaking/Behaviour.h"

bool UBehaviour::IsValidSelectionOption() const
{
	return !IsCoolingDown() && AreStartingConditionMet();
}

bool UBehaviour::IsCoolingDown() const
{
	return false;
}

bool UBehaviour::AreStartingConditionMet() const
{
	return true;
}

float UBehaviour::GetSelectionScore() const
{
	return 1.0f;
}
