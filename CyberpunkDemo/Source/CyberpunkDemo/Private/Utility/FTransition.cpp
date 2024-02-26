// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/FTransition.h"

UFTransition::UFTransition()
{
}

// Init function that substitutes the constructor
//	here we set:
//	- the state the transition points to
//  - the function used to check if the transition is successful or not, we pass it using a TFUNCTION which is a pointer to a static method in the class that holds the state machine
void UFTransition::Init(const TObjectPtr<UFState> toState)
{
	ToState = toState;
	// Condition = condition;
	//
	// // Bind the condition to a delegate (to call later when the transition is checked from the state machine)
	// OnCheckConditionDelegate.BindLambda(Condition);
}

// Check if the state machine can transition to the state pointed by this transition (by executing the condition bound to the delegate)
bool UFTransition::CheckTransition()
{
	return OnCheckConditionDelegate.Execute();
}
