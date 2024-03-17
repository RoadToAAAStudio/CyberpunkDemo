// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "StateTreeTaskBase.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "StateTreeSelectGoalTask.generated.h"

enum class EBasicEnemyGoal : uint8;

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeSelectGoalTaskInstanceData
{
	GENERATED_BODY()

	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ABasicEnemy> BasicEnemy;
	
	UPROPERTY(EditAnywhere, Category = "Output")
	EBasicEnemyGoal ChosenGoal;
};

/**
 * Check if the selected goal exists and, if it is, prepare
 */
USTRUCT(meta = (DisplayName = "Select Goal"))
struct CYBERPUNKDEMO_API FStateTreeSelectGoalTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeSelectGoalTaskInstanceData;
   
	FStateTreeSelectGoalTask() = default;
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
