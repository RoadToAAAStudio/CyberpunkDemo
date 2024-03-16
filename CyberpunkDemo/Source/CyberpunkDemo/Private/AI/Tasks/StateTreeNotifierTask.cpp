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
		Cast<IStateTreeNotificationsAcceptor>(Actor)->Execute_AcceptStateTreeNotification(Actor, InstanceData.StateTreeNotifier, InstanceData.DataTable, Transition);
	}
	return EStateTreeRunStatus::Running;
}
