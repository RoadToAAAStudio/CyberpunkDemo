// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Interactables/Doors/Conditions/HighTechCondition.h"

enum class EInspectInput:uint8 {KeepRPressed};

// Sets default values
UHighTechCondition::UHighTechCondition()
{
}

void UHighTechCondition::AddUnlockInput(TArray<EInspectInput>& ArrayInputs)
{
	ArrayInputs.Add(EInspectInput::KeepRPressed);
}

bool UHighTechCondition::Check()
{
	//check if player has high tech to pass the check
	return Super::Check();
}



