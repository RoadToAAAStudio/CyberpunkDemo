// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Interactables/Doors/Conditions/QuickHackCondition.h"


// Sets default values
UQuickHackCondition::UQuickHackCondition()
{
}

void UQuickHackCondition::AddUnlockInput(TArray<EInspectInput>& ArrayInputs)
{
	Super::AddUnlockInput(ArrayInputs);
}

bool UQuickHackCondition::Check()
{
	//check if the player have the quick hack to unlock doors
	return Super::Check();
}

