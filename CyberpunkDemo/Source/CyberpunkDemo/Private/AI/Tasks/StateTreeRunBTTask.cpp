// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/StateTreeRunBTTask.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework/Character.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeRunBTTask)

EStateTreeRunStatus FStateTreeRunBTTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	AAIController* Controller = Cast<AAIController>(InstanceData.Actor->GetController());
	if (!Controller) return EStateTreeRunStatus::Failed;

	UBrainComponent* Brain = Controller->GetBrainComponent();
	if (Brain)
	{
		Brain->StopLogic(FString(""));
	}
	
	bool Success = Controller->RunBehaviorTree(InstanceData.BehaviourTree);
	if (!Success) return EStateTreeRunStatus::Failed;

	return EStateTreeRunStatus::Running;
}
