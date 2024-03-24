// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Sensing/StateTreeBasicEnemyDataPassing.h"
#include "StateTreeExecutionContext.h"
#include "AI/BasicEnemy/BasicEnemyController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeBasicEnemyDataPassing)


void FStateTreeBasicEnemyDataPassing::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	ABasicEnemy* BasicEnemy = InstanceData.BasicEnemy;

	InstanceData.SpawnLocation = BasicEnemy->GetBasicEnemyController()->GetSpawnLocation();
	InstanceData.PatrolSpline = BasicEnemy->GetBasicEnemyController()->GetPatrolSpline();
	InstanceData.PlayerInSightCone = BasicEnemy->GetBasicEnemyController()->GetPlayerInSightCone();
	InstanceData.SensedLocation = BasicEnemy->GetBasicEnemyController()->GetSensedLocation();
	InstanceData.bIsHeardStimulusSet = BasicEnemy->GetBasicEnemyController()->IsSensedLocationSet();
	InstanceData.CoverLocation = BasicEnemy->GetBasicEnemyController()->GetCoverLocation();
	InstanceData.Tags = BasicEnemy->GetBasicEnemyController()->GetTags();
	InstanceData.GeneratedGoals = BasicEnemy->GetBasicEnemyController()->GetCurrentGeneratedGoals();
}
