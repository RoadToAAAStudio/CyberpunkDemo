// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/StateMachine.h"

#include "EntitySystem/MovieSceneEntitySystemRunner.h"

UStateMachine::UStateMachine()
{
}

void UStateMachine::Init(const TObjectPtr<UFState> StartingState)
{
	CurrentState = StartingState;
}

void UStateMachine::AddState(const TObjectPtr<UFState> NewState)
{
	States.Add(NewState);
}

void UStateMachine::AddTransition(const TObjectPtr<UFState> FromState, const TObjectPtr<UFTransition> NewTransition)
{
	Transitions[FromState].Add(NewTransition);
}


void UStateMachine::Tick()
{
	for (auto e : Transitions[CurrentState])
	{
		if (e->CheckTransition())
		{
			CurrentState->ExitState();

			CurrentState = e->ToState;

			CurrentState->EnterState();

			break;
		}
	}

	CurrentState->Tick();
}
