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
	TSet<EBasicEnemyGoal> GeneratedGoals;

	UPROPERTY(VisibleAnywhere, Category="Output")
	TObjectPtr<ASplineContainer> PatrolSpline;
	
	UPROPERTY(VisibleAnywhere, Category="Output")
	TObjectPtr<AActor> Player;
};

/**
 * Evaluator that generates data for both the Basic Enemy and the State Tree (Data on the Basic Enemy is duplicated to be debugged)
 * Generates Goals from Knowledge and provide all data for every goal generated
 * Output data has to be check only if the corresponding goal is possible
 */
USTRUCT(meta = (DisplayName = "Basic Enemy Goal Generator"))
struct CYBERPUNKDEMO_API FStateTreeBasicEnemyEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeBasicEnemyEvaluatorInstanceData;

	static void AddNewGoal(ABasicEnemy* BasicEnemy, FInstanceDataType& InstanceData, EBasicEnemyGoal Goal);
	static void RemoveGoal(ABasicEnemy* BasicEnemy, FInstanceDataType& InstanceData, EBasicEnemyGoal Goal);
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual void TreeStart(FStateTreeExecutionContext& Context) const override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
