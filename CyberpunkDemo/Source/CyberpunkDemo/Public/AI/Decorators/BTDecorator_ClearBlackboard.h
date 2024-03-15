// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_ClearBlackboard.generated.h"

/**
 * Clear a Blackboard entry
 */
UCLASS()
class CYBERPUNKDEMO_API UBTDecorator_ClearBlackboard : public UBTDecorator_BlackboardBase
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;
};
