// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/Transition.h"

UTransition::UTransition()
{
}

// Init function that substitutes the constructor
//	here we set the state the transition points to
void UTransition::Init(const TObjectPtr<UState> toState)
{
	ToState = toState;
}

// Check if the state machine can transition to the state pointed by this transition (by executing the condition bound to the delegate)
bool UTransition::CheckTransition()
{
	if (OnCheckConditionDelegate.IsBound())
	{
		return OnCheckConditionDelegate.Execute();
	}

	UE_LOG(LogTemp, Warning, TEXT("Transition to %s is not bound"), *ToState->GetName());
	return false;
}
