// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveAlongSpline.generated.h"

class UCharacterMovementComponent;
class USplineComponent;

struct FBTMoveAlongSplineMemory
{
	ACharacter* Owner;
	UCharacterMovementComponent* MovementComponent;
	USplineComponent* Spline;
	float PositionOnSpline;
};

/**
 *  BT Task that moves a Character along a spline contained in an object entry in the blackboard.
 *  Movement speed is get from the Character movement component
 */
UCLASS()
class CYBERPUNKDEMO_API UBTTask_MoveAlongSpline : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
	float StartToSlowDownDistance;
	
	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
	bool bStartsFromEnd;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	virtual uint16 GetInstanceMemorySize() const override;
};
