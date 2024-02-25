// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Doors/Conditions/QuickHackCondition.h"


// Sets default values
UQuickHackCondition::UQuickHackCondition()
{
}

bool UQuickHackCondition::Check()
{
	//check if the player have the quick hack to unlock doors
	return Super::Check();
}

