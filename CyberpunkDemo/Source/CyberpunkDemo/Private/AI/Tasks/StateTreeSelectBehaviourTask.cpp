// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/StateTreeSelectBehaviourTask.h"
#include "StateTreeExecutionContext.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeSelectBehaviourTask)

EStateTreeRunStatus FStateTreeSelectBehaviourTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ABasicEnemy* BasicEnemy = InstanceData.BasicEnemy;

	// Binary Filter
	TSet<EBasicEnemyBehaviour> SupportedBehaviour = BasicEnemy->SupportedBehaviours;
	TSet<EBasicEnemyBehaviour> BehaviourToFilter;
	// The possible behaviour are filtered by state
	switch (BasicEnemy->CurrentState)
	{
		case EBasicEnemyState::Unaware:
		{
			BehaviourToFilter.Add(EBasicEnemyBehaviour::BlindInvestigation);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::Shoot);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::QuickMeleeAttack);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::ThrowGrenade);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::MoveToCover);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::ShootFromCover);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::ThrowGrenadeFromCover);
		}
		break;	
		
		case EBasicEnemyState::Combat:
		{
			BehaviourToFilter.Add(EBasicEnemyBehaviour::Idle);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::ReturnToSpawnPoint);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::Patrol);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::BlindInvestigation);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::Investigation);
		}
		break;
		
		case EBasicEnemyState::Alerted:
		{
			BehaviourToFilter.Add(EBasicEnemyBehaviour::Idle);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::ReturnToSpawnPoint);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::Patrol);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::Shoot);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::QuickMeleeAttack);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::ThrowGrenade);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::MoveToCover);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::ShootFromCover);
			BehaviourToFilter.Add(EBasicEnemyBehaviour::ThrowGrenadeFromCover);
		}
		break;
	}

	// Other filtering
	// Filter Return To SpawnPoint
	if (FVector::Distance(BasicEnemy->GetActorLocation(), InstanceData.SpawnLocation) < 5.0f)
	{
		BehaviourToFilter.Add(EBasicEnemyBehaviour::ReturnToSpawnPoint);
	}

	// Filter Patrol
	if (!InstanceData.GeneratedGoals.Contains(EBasicEnemyGoal::Patrol))
	{
		BehaviourToFilter.Add(EBasicEnemyBehaviour::Patrol);
	}

	// Filter Investigations
	if (!InstanceData.GeneratedGoals.Contains(EBasicEnemyGoal::Search))
	{
		BehaviourToFilter.Add(EBasicEnemyBehaviour::BlindInvestigation);
		BehaviourToFilter.Add(EBasicEnemyBehaviour::Investigation);
	}

	// Filter Shoot
	if (!InstanceData.GeneratedGoals.Contains(EBasicEnemyGoal::Combat) ||
		      InstanceData.PlayerDistance > BasicEnemy->MaxDistanceToShoot ||
		      InstanceData.PlayerDistance < BasicEnemy->MinDistanceToShoot)
	{
		BehaviourToFilter.Add(EBasicEnemyBehaviour::Shoot);
	}

	// Filter QuickMelee Attack
	if (!InstanceData.GeneratedGoals.Contains(EBasicEnemyGoal::Combat) ||
		  InstanceData.PlayerDistance > BasicEnemy->MaxDistanceToQuickMeleeAttack)
	{
		BehaviourToFilter.Add(EBasicEnemyBehaviour::QuickMeleeAttack);
	}
	
	// TODO Other Filtering
	
	BasicEnemy->CurrentFilteredBehaviours = BehaviourToFilter;
	
	SupportedBehaviour = SupportedBehaviour.Difference(BehaviourToFilter);

	// Check results
	if (SupportedBehaviour.Num() == 0)
	{
		// Default behaviour if nothing is possible
		InstanceData.ChosenBehaviour = EBasicEnemyBehaviour::None;
		BasicEnemy->CurrentChosenBehaviour = EBasicEnemyBehaviour::None;
	}
	else if (SupportedBehaviour.Num() == 1)
	{
		// Trivial case
		for (auto& Behaviour : SupportedBehaviour)
		{
			InstanceData.ChosenBehaviour = Behaviour;
			BasicEnemy->CurrentChosenBehaviour = Behaviour;
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
				BehaviourScore = 1;
				break;
			case EBasicEnemyBehaviour::ReturnToSpawnPoint:
				BehaviourScore = 10;
				break;
			case EBasicEnemyBehaviour::Patrol:
				BehaviourScore = 20;
				break;
			case EBasicEnemyBehaviour::BlindInvestigation:
				BehaviourScore = 30;
				break;
			case EBasicEnemyBehaviour::Investigation:
				BehaviourScore = 40;
				break;
			case EBasicEnemyBehaviour::Shoot:
				BehaviourScore = 50;
				break;
			case EBasicEnemyBehaviour::QuickMeleeAttack:
				BehaviourScore = 60;
				break;
			case EBasicEnemyBehaviour::ThrowGrenade:
				BehaviourScore = 70;
				break;
			case EBasicEnemyBehaviour::MoveToCover:
				BehaviourScore = 80;
				break;
			case EBasicEnemyBehaviour::ShootFromCover:
				BehaviourScore = 90;
				break;
			case EBasicEnemyBehaviour::ThrowGrenadeFromCover:
				BehaviourScore = 100;
				break;
			}

			if (BehaviourScore > BestScore)
			{
				BestBehaviour = Behaviour;
				BestScore = BehaviourScore;
			}
		}

		InstanceData.ChosenBehaviour = BestBehaviour;
		BasicEnemy->CurrentChosenBehaviour = BestBehaviour;
	}
	
	return EStateTreeRunStatus::Running;
}
