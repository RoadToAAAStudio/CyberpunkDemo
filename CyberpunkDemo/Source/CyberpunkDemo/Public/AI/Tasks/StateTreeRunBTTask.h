// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeRunBTTask.generated.h"

class UBehaviorTree;

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeRunBTTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<ACharacter> Actor;
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<UBehaviorTree> BehaviourTree;
};

/**
 * 
 */
USTRUCT(meta = (DisplayName = "Run Behaviour Tree"))
struct CYBERPUNKDEMO_API FStateTreeRunBTTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeRunBTTaskInstanceData;
	
	FStateTreeRunBTTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,	const FStateTreeTransitionResult& Transition) const override;
};
