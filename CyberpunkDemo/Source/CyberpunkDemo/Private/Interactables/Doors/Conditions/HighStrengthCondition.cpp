// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Doors/Conditions/HighStrengthCondition.h"


// Sets default values
UHighStrengthCondition::UHighStrengthCondition()
{
}

bool UHighStrengthCondition::Check()
{
	//check if player has high strength to pass the check
	return Super::Check();
}

