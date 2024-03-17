// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Sensing/StateTreeBasicEnemyEvaluator.h"
#include "StateTreeExecutionContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeBasicEnemyEvaluator)

void FStateTreeBasicEnemyEvaluator::AddNewGoal(ABasicEnemy* BasicEnemy, FInstanceDataType& InstanceData, EBasicEnemyGoal Goal)
{
	BasicEnemy->CurrentGeneratedGoals.Add(Goal);
	InstanceData.GeneratedGoals.Add(Goal);
}

void FStateTreeBasicEnemyEvaluator::RemoveGoal(ABasicEnemy* BasicEnemy, FInstanceDataType& InstanceData, EBasicEnemyGoal Goal)
{
	BasicEnemy->CurrentGeneratedGoals.Remove(Goal);
	InstanceData.GeneratedGoals.Remove(Goal);
}

void FStateTreeBasicEnemyEvaluator::TreeStart(FStateTreeExecutionContext& Context) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
}

void FStateTreeBasicEnemyEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ABasicEnemy* BasicEnemy = InstanceData.BasicEnemy;
	if (!BasicEnemy) return;

	// Idle Goal
	AddNewGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Idle);
	
	// Patrol Goal
	if (BasicEnemy->PatrolSpline)
	{
		AddNewGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Patrol);
		InstanceData.PatrolSpline = BasicEnemy->PatrolSpline;
	}
	else
	{
		RemoveGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Patrol);
	}
	
	// Combat Goal
	if (BasicEnemy->GetSharedKnowledge()->GetPlayer())
	{
		AddNewGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Combat);
		InstanceData.Player = BasicEnemy->GetSharedKnowledge()->GetPlayer();
	}
	else
	{
		RemoveGoal(BasicEnemy, InstanceData, EBasicEnemyGoal::Combat);
	}
}
