// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/FTransition.h"

UFTransition::UFTransition()
{
}

void UFTransition::Init(TObjectPtr<UStateInterface> toState)
{
	ToState = toState;
	//OnCheckConditionDelegate.BindLambda(Condition);
}

bool UFTransition::CheckTransition()
{
	return true;
}
