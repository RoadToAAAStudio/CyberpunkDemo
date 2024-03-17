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
	TArray<EBasicEnemyGoal> PossibleGoals;

};

/**
 * Evaluator that generates goals for the Basic Enemy agent
 */
USTRUCT(meta = (DisplayName = "Basic Enemy Goal Generator"))
struct CYBERPUNKDEMO_API FStateTreeBasicEnemyEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeBasicEnemyEvaluatorInstanceData;

	static void AddNewGoal(ABasicEnemy* BasicEnemy, FInstanceDataType& InstanceData, EBasicEnemyGoal Goal);
	static void RemoveGoal(ABasicEnemy* BasicEnemy, FInstanceDataType& InstanceData, EBasicEnemyGoal Goal);
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
