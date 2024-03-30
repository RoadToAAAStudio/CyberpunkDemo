#pragma once
#include "StateTreeConditionBase.h"
#include "StateTreeConditionCompareEnumBehaviour.generated.h"

enum class EBasicEnemyBehaviour : uint8;

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeCompareEnumBehaviourConditionInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input", meta=(AllowAnyBinding))
	EBasicEnemyBehaviour Left;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	EBasicEnemyBehaviour Right;
};

/**
 * Condition comparing two enums.
 */
USTRUCT(DisplayName = "Behaviour Enum Compare")
struct CYBERPUNKDEMO_API FStateTreeCompareEnumBehaviourCondition : public FStateTreeConditionCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeCompareEnumBehaviourConditionInstanceData;

	FStateTreeCompareEnumBehaviourCondition() = default;
	explicit FStateTreeCompareEnumBehaviourCondition(const EStateTreeCompare InInverts)
		: bInvert(InInverts == EStateTreeCompare::Invert)
	{}

	FStateTreeCompareEnumBehaviourCondition(const bool bInInverts)
		: bInvert(bInInverts)
	{}

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	

	UPROPERTY(EditAnywhere, Category = "Parameter")
	bool bInvert = false;
};