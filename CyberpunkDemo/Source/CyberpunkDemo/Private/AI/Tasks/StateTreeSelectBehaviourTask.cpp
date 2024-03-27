// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/StateTreeSelectBehaviourTask.h"
#include "StateTreeExecutionContext.h"
#include "AI/BasicEnemy/BasicEnemy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeSelectBehaviourTask)

EStateTreeRunStatus FStateTreeSelectBehaviourTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ABasicEnemy* BasicEnemy = InstanceData.BasicEnemy;
	
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
		InstanceData.ChosenBehaviour = EBasicEnemyBehaviour::Idle;
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
		EBasicEnemyBehaviour BestBehaviour = EBasicEnemyBehaviour::Idle;
		int BestScore = -1;

		for (auto& Behaviour : SupportedBehaviour)
		{
			int BehaviourScore = -1;
			switch (Behaviour)
			{
			case EBasicEnemyBehaviour::Idle:
				break;
			case EBasicEnemyBehaviour::ReturnToSpawnPoint:
				break;
			case EBasicEnemyBehaviour::Patrol:
				break;
			case EBasicEnemyBehaviour::Investigation:
				break;
			case EBasicEnemyBehaviour::BlindInvestigation:
				break;
			case EBasicEnemyBehaviour::Shoot:
				break;
			case EBasicEnemyBehaviour::QuickMeleeAttack:
				break;
			case EBasicEnemyBehaviour::ThrowGrenade:
				break;
			case EBasicEnemyBehaviour::MoveToCover:
				break;
			case EBasicEnemyBehaviour::ShootFromCover:
				break;
			case EBasicEnemyBehaviour::ThrowGrenadeFromCover:
				break;
			}

			if (BehaviourScore > BestScore)
			{
				BestBehaviour = Behaviour;
				BestScore = BehaviourScore;
			}
		}

		InstanceData.ChosenBehaviour = BestBehaviour;

	}
	
	return EStateTreeRunStatus::Running;
}
