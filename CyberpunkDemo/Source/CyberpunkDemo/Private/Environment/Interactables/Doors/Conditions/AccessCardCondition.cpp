// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/Interactables/Doors/Conditions/AccessCardCondition.h"


// Sets default values
UAccessCardCondition::UAccessCardCondition()
{
}



bool UAccessCardCondition::Check()
{
	//check if the player have the required access card
	return Super::Check();
}

