// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveAlongSpline.generated.h"

struct FBTMoveAlongSplineMemory
{
	float PositionOnSpline = 0.0f;
};

/**
 *  BT Task that moves a Character along a spline contained in an object entry in the blackboard.
 *  Movement speed is get from the Character movement component
 */
UCLASS()
class CYBERPUNKDEMO_API UBTTask_MoveAlongSpline : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	
private:
	TObjectPtr<USplineComponent> Spline;
	
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
