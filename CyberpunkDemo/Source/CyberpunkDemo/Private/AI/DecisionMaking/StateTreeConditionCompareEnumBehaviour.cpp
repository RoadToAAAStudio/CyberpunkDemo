#include "AI/DecisionMaking/StateTreeConditionCompareEnumBehaviour.h"
#include "StateTreeExecutionContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeConditionCompareEnumBehaviour)

bool FStateTreeCompareEnumBehaviourCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	return (InstanceData.Left == InstanceData.Right) ^ bInvert;
}
