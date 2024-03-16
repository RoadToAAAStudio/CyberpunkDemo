// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "StateTreeBasicEnemyEvaluator.generated.h"

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeBasicEnemyEvaluatorInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Context")
	TObjectPtr<ABasicEnemy> BasicEnemy;

	UPROPERTY(VisibleAnywhere, Category="Output")
	bool bInvestigationGoal = false;
	
	UPROPERTY(VisibleAnywhere, Category="Output")
	bool bPatrolGoal = false;
	
	UPROPERTY(VisibleAnywhere, Category="Output")
	bool bCombatGoal = false;
};

/**
 * Evaluator that generates goals for the Basic Enemy agent
 */
USTRUCT(meta = (DisplayName = "Basic Enemy Goal Generator"))
struct CYBERPUNKDEMO_API FStateTreeBasicEnemyEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeBasicEnemyEvaluatorInstanceData;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
