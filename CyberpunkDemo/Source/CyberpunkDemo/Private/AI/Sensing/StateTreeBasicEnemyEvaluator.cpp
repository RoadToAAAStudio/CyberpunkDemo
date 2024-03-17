// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Sensing/StateTreeBasicEnemyEvaluator.h"
#include "StateTreeExecutionContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeBasicEnemyEvaluator)

void FStateTreeBasicEnemyEvaluator::AddNewGoal(ABasicEnemy* BasicEnemy, FInstanceDataType& InstanceData, EBasicEnemyGoal Goal)
{
	BasicEnemy->CurrentPossibleGoals.Add(Goal);
	InstanceData.PossibleGoals.Add(Goal);
}

void FStateTreeBasicEnemyEvaluator::RemoveGoal(ABasicEnemy* BasicEnemy, FInstanceDataType& InstanceData, EBasicEnemyGoal Goal)
{
	BasicEnemy->CurrentPossibleGoals.Remove(Goal);
	InstanceData.PossibleGoals.Remove(Goal);
}

void FStateTreeBasicEnemyEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ABasicEnemy* BasicEnemy = InstanceData.BasicEnemy;
	if (!BasicEnemy) return;

	// Patrol Goal
	if (BasicEnemy->PatrolSpline)
	{
		AddNewGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Patrol);
	}
	else
	{
		RemoveGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Patrol);
	}
	
	// Combat Goal
	if (BasicEnemy->GetSharedKnowledge()->GetPlayer())
	{
		AddNewGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Combat);
	}
	else
	{
		RemoveGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Combat);
	}
}
