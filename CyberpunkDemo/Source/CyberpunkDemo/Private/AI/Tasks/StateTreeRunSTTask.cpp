#include "AI/Tasks/StateTreeRunSTTask.h"
#include "VisualLogger/VisualLogger.h"
#include "StateTreeExecutionContext.h"

#define STATETREE_LOG(Verbosity, Format, ...) UE_VLOG(Context.GetOwner(), LogStateTree, Verbosity, Format, ##__VA_ARGS__)
#define STATETREE_CLOG(Condition, Verbosity, Format, ...) UE_CVLOG((Condition), Context.GetOwner(), LogStateTree, Verbosity, Format, ##__VA_ARGS__)

EStateTreeRunStatus FStateTreeRunSTTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    if (!InstanceData.StateTreeRef.IsValid())
    {
        STATETREE_LOG(Error, TEXT("%s: StateTree asset is not set, cannot enter subtree task state."), ANSI_TO_TCHAR(__FUNCTION__));
        return EStateTreeRunStatus::Failed;
    }

    FStateTreeExecutionContext ChildContext(*Context.GetOwner(), *InstanceData.StateTreeRef.GetStateTree(), InstanceData.InstanceData);
    if (SetContextRequirements(Context, ChildContext, true))
    {
        ChildContext.SetParameters(InstanceData.StateTreeRef.GetParameters());
        return ChildContext.Start();
    }

    return EStateTreeRunStatus::Failed;
}

EStateTreeRunStatus FStateTreeRunSTTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    FStateTreeExecutionContext ChildContext(*Context.GetOwner(), *InstanceData.StateTreeRef.GetStateTree(), InstanceData.InstanceData);
    if (SetContextRequirements(Context, ChildContext, true))
    {
        return ChildContext.Tick(DeltaTime);
    }

    return EStateTreeRunStatus::Failed;
}

void FStateTreeRunSTTask::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    FStateTreeExecutionContext ChildContext(*Context.GetOwner(), *InstanceData.StateTreeRef.GetStateTree(), InstanceData.InstanceData);
    if (SetContextRequirements(Context, ChildContext, true))
    {
        ChildContext.SetParameters(InstanceData.StateTreeRef.GetParameters());
        ChildContext.Stop();
    }
}

bool FStateTreeRunSTTask::SetContextRequirements(FStateTreeExecutionContext& Context, FStateTreeExecutionContext& ChildContext, bool bLogErrors)
{
    if (!ChildContext.IsValid())
    {
        return false;
    }

    // Make sure the schema of the child is at least a super class of the parent tree so things can copy correctly
    if (!Context.GetStateTree()->GetSchema()->IsA(ChildContext.GetStateTree()->GetSchema()->GetClass()))
    {
        return false;
    }

    {
        TMap<FName, const FStateTreeExternalDataDesc*> ContextDataNameToDescriptor;

        // Build lookups of named ItemDescriptors in the parent context so they can be transferred to the child
        for (const FStateTreeExternalDataDesc& ItemDesc : Context.GetContextDataDescs())
        {
            if (ItemDesc.Name != NAME_None)
            {
                ContextDataNameToDescriptor.Add(ItemDesc.Name, &ItemDesc);
            }
        }

        // Copy data with the same name from parent to child
        for (const FStateTreeExternalDataDesc& ItemDesc : ChildContext.GetContextDataDescs())
        {
            if (auto Desc = ContextDataNameToDescriptor.Find(ItemDesc.Name))
            {
                ChildContext.SetExternalData(ItemDesc.Handle, Context.GetExternalDataView((*Desc)->Handle));
            }
        }
    }

    {
        TMap<const UStruct*, const FStateTreeExternalDataDesc*> ExternalDataStructToDescriptor;
    
        // Build lookups of named ItemDescriptors in the parent context so they can be transferred to the child
        for (const FStateTreeExternalDataDesc& ItemDesc : Context.GetExternalDataDescs())
        {
            if (ItemDesc.Struct)
            {
                ExternalDataStructToDescriptor.Add(ItemDesc.Struct, &ItemDesc);
            }
        }
    
        // Copy data with the same struct from parent to child
        for (const FStateTreeExternalDataDesc& ItemDesc : ChildContext.GetExternalDataDescs())
        {
            if (auto Desc = ExternalDataStructToDescriptor.Find(ItemDesc.Struct))
            {
                ChildContext.SetExternalData(ItemDesc.Handle, Context.GetExternalDataView((*Desc)->Handle));
            }
        }
    }
    
    bool bResult = ChildContext.AreExternalDataViewsValid();
    
    if (!bResult && bLogErrors)
    {
        STATETREE_LOG(Error, TEXT("%s: Missing external data requirements. StateTree will not update."), ANSI_TO_TCHAR(__FUNCTION__));
    }
    
    // Pass along events
    if (bResult)
    {
        Context.ForEachEvent([&ChildContext](const FStateTreeEvent& Event)
        {
            ChildContext.SendEvent(Event.Tag, Event.Payload, Event.Origin);
            return EStateTreeLoopEvents::Next;
        });
    }

    return bResult;
}

#undef STATETREE_LOG
#undef STATETREE_CLOG
