// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Interactables/Doors/Conditions/HighStrengthCondition.h"

// Sets default values
UHighStrengthCondition::UHighStrengthCondition()
{
}

void UHighStrengthCondition::AddUnlockInput(TArray<EInspectInput>& ArrayInputs)
{
	if (ArrayInputs.Contains(EInspectInput::PressF))
	{
		ArrayInputs.Add(EInspectInput::KeepFPressed);
	}
	else
	{
		ArrayInputs.Add(EInspectInput::PressF);
	}
}

bool UHighStrengthCondition::Check()
{
	//check if player has high strength to pass the check
	return Super::Check();
}

