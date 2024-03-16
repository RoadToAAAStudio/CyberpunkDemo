#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "StateTreeNotifierTask.generated.h"

class UStateTree;

USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeNotifierTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> Actor;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<UStateTree> StateTreeNotifier;
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<UDataTable> DataTable;
};

/**
 * 
 */
USTRUCT(meta = (DisplayName = "State Tree Notifier"))
struct CYBERPUNKDEMO_API FStateTreeNotifierTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeNotifierTaskInstanceData;
	
	FStateTreeNotifierTask() = default;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,	const FStateTreeTransitionResult& Transition) const override;
};
