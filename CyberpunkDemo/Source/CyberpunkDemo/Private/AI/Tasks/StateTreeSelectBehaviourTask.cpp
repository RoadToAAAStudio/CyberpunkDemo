// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/StateTreeSelectBehaviourTask.h"
#include "StateTreeExecutionContext.h"
#include "AI/BasicEnemy/BasicEnemy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeSelectBehaviourTask)

EStateTreeRunStatus FStateTreeSelectBehaviourTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ABasicEnemy* BasicEnemy = InstanceData.BasicEnemy;
	if(!BasicEnemy) return EStateTreeRunStatus::Running;
	
	TSet<EBasicEnemyBehaviour> SupportedBehaviour = BasicEnemy->SupportedBehaviours;

	// The possible behaviour are filtered by state
	switch (BasicEnemy->CurrentState)
	{
	case EBasicEnemyState::Unaware:
		SupportedBehaviour = SupportedBehaviour.Difference(InstanceData.UnawareBehavioursFilter);
		break;
		
	case EBasicEnemyState::Combat:
		SupportedBehaviour = SupportedBehaviour.Difference(InstanceData.CombatBehavioursFilter);
		break;
		
	case EBasicEnemyState::Alerted:
		SupportedBehaviour = SupportedBehaviour.Difference(InstanceData.AlertedBehavioursFilter);
		break;
	}

	// Check results
	if (SupportedBehaviour.Num() == 0)
	{
		// Default behaviour if nothing is possible
		InstanceData.ChosenBehaviour = EBasicEnemyBehaviour::None;
	}
	else if (SupportedBehaviour.Num() == 1)
	{
		// Trivial case
		for (auto& Behaviour : SupportedBehaviour)
		{
			InstanceData.ChosenBehaviour = Behaviour;
			break;
		}
	}
	else
	{
		// Assign to each one a score
		EBasicEnemyBehaviour BestBehaviour = EBasicEnemyBehaviour::None;
		int BestScore = -1;

		for (auto& Behaviour : SupportedBehaviour)
		{
			int BehaviourScore = -1;
			switch (Behaviour)
			{
			case EBasicEnemyBehaviour::Investigation:
				break;
			case EBasicEnemyBehaviour::Patrol:
				break;
			//case EBasicEnemyBehaviour::Combat:
				break;
			}
		}

		InstanceData.ChosenBehaviour = BestBehaviour != InstanceData.ChosenBehaviour? BestBehaviour : InstanceData.ChosenBehaviour;

	}
	
	return EStateTreeRunStatus::Succeeded;
}
