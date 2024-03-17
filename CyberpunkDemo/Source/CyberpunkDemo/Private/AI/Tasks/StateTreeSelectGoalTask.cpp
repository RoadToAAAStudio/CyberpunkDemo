// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/StateTreeSelectGoalTask.h"
#include "StateTreeExecutionContext.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeSelectGoalTask)

EStateTreeRunStatus FStateTreeSelectGoalTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ABasicEnemy* BasicEnemy = InstanceData.BasicEnemy;
	if(!BasicEnemy) return EStateTreeRunStatus::Running;
	
	TArray<EBasicEnemyGoal> PossibleGoals = BasicEnemy->CurrentPossibleGoals;

	// The possible goals are filtered by state and then chosen using a priority
	switch (BasicEnemy->CurrentState)
	{
	case EBasicEnemyState::Unaware:
		if (PossibleGoals.Contains(EBasicEnemyGoal::Investigation))
		{
			InstanceData.ChosenGoal = EBasicEnemyGoal::Investigation;
		}
		else if (PossibleGoals.Contains(EBasicEnemyGoal::Patrol))
		{
			InstanceData.ChosenGoal = EBasicEnemyGoal::Patrol;
		}
		else
		{
			InstanceData.ChosenGoal = EBasicEnemyGoal::None;
		}
		break;
	case EBasicEnemyState::Combat:
		if (PossibleGoals.Contains(EBasicEnemyGoal::Combat))
		{
			InstanceData.ChosenGoal = EBasicEnemyGoal::Combat;
		}
		break;
	case EBasicEnemyState::Alerted:
		if (PossibleGoals.Contains(EBasicEnemyGoal::Investigation))
		{
			InstanceData.ChosenGoal = EBasicEnemyGoal::Investigation;
		}
		else
		{
			InstanceData.ChosenGoal = EBasicEnemyGoal::None;
		}
		break;
	case EBasicEnemyState::Max:
		break;
	}
	
	return EStateTreeRunStatus::Running;
}
