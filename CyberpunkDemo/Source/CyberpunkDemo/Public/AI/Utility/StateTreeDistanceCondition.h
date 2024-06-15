#pragma once
#include "AITypes.h"
#include "StateTreeConditionBase.h"
#include "StateTreeDistanceCondition.generated.h"

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeDistanceConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FVector Source = FVector(EForceInit::ForceInitToZero);

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<AActor> SourceActor;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	FVector Target = FVector(EForceInit::ForceInitToZero);

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	double Distance = 0.0;
};
STATETREE_POD_INSTANCEDATA(FStateTreeDistanceConditionInstanceData);

/**
 * Condition comparing distance between two vectors.
 */
USTRUCT(DisplayName = "Distance Condition")
struct CYBERPUNKDEMO_API FStateTreeDistanceCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStateTreeDistanceConditionInstanceData;

	FStateTreeDistanceCondition() = default;
	explicit FStateTreeDistanceCondition(const EGenericAICheck InOperator, const EStateTreeCompare InInverts = EStateTreeCompare::Default)
		: bInvert(InInverts == EStateTreeCompare::Invert)
		, Operator(InOperator)
	{}
	
	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	UPROPERTY(EditAnywhere, Category = "Condition")
	bool bInvert = false;

	UPROPERTY(EditAnywhere, Category = "Condition")
	EGenericAICheck Operator = EGenericAICheck::Equal;
};