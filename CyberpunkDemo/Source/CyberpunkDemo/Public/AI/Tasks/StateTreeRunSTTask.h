#pragma once

#include "CoreMinimal.h"
#include "StateTreeReference.h"
#include "StateTreeInstanceData.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeRunSTTask.generated.h"

enum class EBasicEnemyBehaviour : uint8;
/**
 * FSubtreeStateTreeTask instance data
 * @see FSubtreeStateTreeTask
 */
USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeRunSTTaskInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Input")
	EBasicEnemyBehaviour ChosenBehaviour;
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TMap<EBasicEnemyBehaviour, FStateTreeReference> Behaviours;

	FStateTreeReference StateTreeRef;
    // UPROPERTY(EditAnywhere, Category = "Parameter", meta=(Schema="/Script/GameplayStateTreeModule.StateTreeComponentSchema"))
    // FStateTreeReference StateTreeRef;

    UPROPERTY(Transient)
    FStateTreeInstanceData InstanceData;
};

/*
 * Runs a sub state tree asset whose schema class should at least be a super class of the parent's schema
 */
USTRUCT(meta = (DisplayName = "Run Chosen Behaviour"))
struct CYBERPUNKDEMO_API FStateTreeRunSTTask : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FStateTreeRunSTTaskInstanceData;

protected:
    virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

    static bool SetContextRequirements(FStateTreeExecutionContext& Context, FStateTreeExecutionContext& ChildContext, bool bLogErrors = false);
};
