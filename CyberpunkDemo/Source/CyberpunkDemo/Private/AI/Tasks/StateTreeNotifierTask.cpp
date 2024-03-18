#include "AI/Tasks/StateTreeNotifierTask.h"
#include "StateTreeExecutionContext.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeNotifierTask)

EStateTreeRunStatus FStateTreeNotifierTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	AActor* Actor = InstanceData.Actor;
	
	if (Cast<IStateTreeNotificationsAcceptor>(Actor))
	{
		FName SourceStateName = Context.GetStateTree()->GetStates()[Transition.SourceState.Index].Name;
		FName CurrentStateName = Context.GetStateTree()->GetStates()[Transition.CurrentState.Index].Name;
		Cast<IStateTreeNotificationsAcceptor>(Actor)->Execute_AcceptStateTreeNotification(Actor, SourceStateName, CurrentStateName);
	}
	return EStateTreeRunStatus::Running;
}
