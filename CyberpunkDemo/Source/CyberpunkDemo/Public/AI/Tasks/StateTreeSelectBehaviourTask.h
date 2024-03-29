// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "StateTreeTaskBase.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "StateTreeSelectBehaviourTask.generated.h"

enum class EBasicEnemyGoal : uint8;

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeSelectBehaviourTaskInstanceData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ABasicEnemy> BasicEnemy;

	// Data passing from Knowledge
	UPROPERTY(VisibleAnywhere, Category="Input")
	FVector SpawnLocation = FVector(0, 0, 0);
	
	UPROPERTY(VisibleAnywhere, Category="Input")
	const ASplineContainer* PatrolSpline = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Input")
	const ACharacter* PlayerInSightCone = nullptr;
	
	UPROPERTY(VisibleAnywhere, Category="Input")
	FVector SensedLocation = FVector();

	UPROPERTY(VisibleAnywhere, Category="Input")
	bool bIsHeardStimulusSet = false;

	UPROPERTY(VisibleAnywhere, Category="Input")
	FVector CoverLocation = FVector(0, 0, 0);

	UPROPERTY(VisibleAnywhere, Category="Input")
	bool bIsCoverLocationSet = false;
	
	UPROPERTY(VisibleAnywhere, Category="Input")
	FGameplayTagContainer Tags = FGameplayTagContainer();

	UPROPERTY(VisibleAnywhere, Category="Input")
	TSet<EBasicEnemyGoal> GeneratedGoals = TSet<EBasicEnemyGoal>();

	UPROPERTY(VisibleAnywhere, Category="Input")
	float PlayerDistance;
	
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
