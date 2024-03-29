// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "StateTreeBasicEnemyDataPassing.generated.h"

struct FAIStimulus;
enum class EBasicEnemyGoal : uint8;

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeBasicEnemyDataPassingInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Context")
	TObjectPtr<ABasicEnemy> BasicEnemy;

	// Data passing from Knowledge
	UPROPERTY(VisibleAnywhere, Category="Output")
	FVector SpawnLocation = FVector(0, 0, 0);
	
	UPROPERTY(VisibleAnywhere, Category="Output")
	const ASplineContainer* PatrolSpline = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Output")
	const ACharacter* PlayerInSightCone = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category="Output")
	FVector SensedLocation = FVector();

	UPROPERTY(VisibleAnywhere, Category="Output")
	bool bIsHeardStimulusSet = false;

	UPROPERTY(VisibleAnywhere, Category="Output")
	FVector CoverLocation = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere, Category="Output")
	bool bIsCoverLocationSet = false;
	
	UPROPERTY(VisibleAnywhere, Category="Output")
	FGameplayTagContainer Tags = FGameplayTagContainer();

	UPROPERTY(VisibleAnywhere, Category="Output")
	TSet<EBasicEnemyGoal> GeneratedGoals = TSet<EBasicEnemyGoal>();

	// Other generated data
	UPROPERTY(VisibleAnywhere, Category="Output")
	float PlayerDistance;
};

/**
 * Evaluator that pass data from knowledge to the State Tree.
 * It generates also some useful data for the decision making
 */
USTRUCT(meta = (DisplayName = "Basic Enemy Data passing"))
struct CYBERPUNKDEMO_API FStateTreeBasicEnemyDataPassing : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeBasicEnemyDataPassingInstanceData;
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
};
