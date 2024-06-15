#include "AI/Utility/StateTreeDistanceCondition.h"
#include "StateTreeExecutionContext.h"

using namespace UE::StateTree;

bool FStateTreeDistanceCondition::TestCondition(FStateTreeExecutionContext& Context) const
{
	const FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	FVector SourceLocation = InstanceData.SourceActor ? InstanceData.SourceActor->GetActorLocation() : InstanceData.Source;
	FVector TargetLocation = InstanceData.TargetActor ? InstanceData.TargetActor->GetActorLocation() : InstanceData.Target;
	
	const float Left = FVector::DistSquared(SourceLocation, TargetLocation);
	const float Right = FMath::Square(InstanceData.Distance);

	bool bResult = false;
	switch (Operator)
	{
	case EGenericAICheck::Equal:
		bResult =  Left == Right;
		break;
	case EGenericAICheck::NotEqual:
		bResult =  Left != Right;
		break;
	case EGenericAICheck::Less:
		bResult =  Left < Right;
		break;
	case EGenericAICheck::LessOrEqual:
		bResult =  Left <= Right;
		break;
	case EGenericAICheck::Greater:
		bResult =  Left > Right;
		break;
	case EGenericAICheck::GreaterOrEqual:
		bResult =  Left >= Right;
		break;
	default:
		ensureMsgf(false, TEXT("Unhandled operator %d"), Operator);
		bResult =  false;
		break;
	}
	
	return bResult ^ bInvert;
}
