// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment//Interfaces/Hackerable.h"


// Add default functionality here for any IHackerable functions that are not pure virtual.
TSet<TSubclassOf<UGameplayAbility>> IHackerable::GetPossibleHacks() const
{
	TSet<TSubclassOf<UGameplayAbility>> PossibleAbilities;
	return PossibleAbilities;
}

void IHackerable::Highlight() const
{
}
