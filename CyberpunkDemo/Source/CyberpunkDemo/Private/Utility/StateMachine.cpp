// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/StateMachine.h"

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

void UStateMachine::Tick()
{
	for (auto e : CurrentState->Transitions)
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
