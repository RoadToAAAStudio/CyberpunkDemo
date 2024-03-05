// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Utility/AIUtility.h"

FName UAIUtility::GetCurrentState(const UStateTree* StateTree, const FStateTreeTransitionResult& Transition)
{
	if (!StateTree)
	{
		return FName(TEXT("Invalid"));
	}
	if (Transition.CurrentState.Index < 0 || Transition.CurrentState.Index >= StateTree->GetStates().Num())
	{
		return FName(TEXT("Invalid"));
	}
	return StateTree->GetStates()[Transition.CurrentState.Index].Name;
}

FName UAIUtility::GetSourceState(const UStateTree* StateTree, const FStateTreeTransitionResult& Transition)
{
	if (!StateTree)
	{
		return FName(TEXT("Invalid"));
	}
	if (Transition.SourceState.Index < 0 || Transition.SourceState.Index >= StateTree->GetStates().Num())
	{
		return FName(TEXT("Invalid"));
	}
	return StateTree->GetStates()[Transition.SourceState.Index].Name;
}


