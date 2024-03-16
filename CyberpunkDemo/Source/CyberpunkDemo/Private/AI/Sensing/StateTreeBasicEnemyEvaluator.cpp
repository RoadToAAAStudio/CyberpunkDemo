// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Sensing/StateTreeBasicEnemyEvaluator.h"
#include "StateTreeExecutionContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeBasicEnemyEvaluator)

void FStateTreeBasicEnemyEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ABasicEnemy* BasicEnemy = InstanceData.BasicEnemy;
	if (!BasicEnemy) return;

	// Patrol Goal
	if (BasicEnemy->PatrolSpline)
	{
		BasicEnemy->bPatrolGoal = true;
		InstanceData.bPatrolGoal = true;
	}
	else
	{
		BasicEnemy->bPatrolGoal = false;
		InstanceData.bPatrolGoal = false;
	}
	
	// Combat Goal
	if (BasicEnemy->GetSharedKnowledge()->GetPlayer())
	{
		BasicEnemy->bCombatGoal = true;
		InstanceData.bCombatGoal = true;
	}
	else
	{
		BasicEnemy->bCombatGoal = false;
		InstanceData.bCombatGoal = false;
	}
}
