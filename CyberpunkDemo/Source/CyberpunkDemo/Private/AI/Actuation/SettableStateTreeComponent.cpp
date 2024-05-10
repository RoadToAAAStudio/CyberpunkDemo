// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Actuation/SettableStateTreeComponent.h"

void USettableStateTreeComponent::SetTree(UStateTree* TargetTree)
{
	StopLogic("StateTree Changed");
	StateTreeRef.SetStateTree(TargetTree);
}