// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Interactables/Doors/Conditions/HighTechCondition.h"


// Sets default values
UHighTechCondition::UHighTechCondition()
{
}

bool UHighTechCondition::Check()
{
	//check if player has high tech to pass the check
	return Super::Check();
}



