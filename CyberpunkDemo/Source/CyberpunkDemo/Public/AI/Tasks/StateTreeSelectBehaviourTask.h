// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "StateTreeTaskBase.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "StateTreeSelectBehaviourTask.generated.h"

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeSelectBehaviourTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ABasicEnemy> BasicEnemy;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TSet<EBasicEnemyBehaviour> UnawareBehavioursFilter;

	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TSet<EBasicEnemyBehaviour> CombatBehavioursFilter;
	
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TSet<EBasicEnemyBehaviour> AlertedBehavioursFilter;
	
	UPROPERTY(EditAnywhere, Category = "Output")
	EBasicEnemyBehaviour ChosenBehaviour;
};

/**
 * Check if the selected goal exists and, if it is, prepare
 */
USTRUCT(meta = (DisplayName = "Select Behaviour"))
struct CYBERPUNKDEMO_API FStateTreeSelectBehaviourTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeSelectBehaviourTaskInstanceData;
   
	FStateTreeSelectBehaviourTask() = default;
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
