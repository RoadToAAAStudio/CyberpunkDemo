#pragma once

#include "CoreMinimal.h"
#include "StateTreeInstanceData.h"
#include "AI/BasicEnemy/BasicEnemyConfigData.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeRunSubTree.generated.h"

enum class EBasicEnemyBehaviour : uint8;
/**
 * FSubtreeStateTreeTask instance data
 * @see FSubtreeStateTreeTask
 */
USTRUCT()
struct CYBERPUNKDEMO_API FStateTreeRunSTTaskInstanceData
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Parameter")
	EBasicEnemyState State = EBasicEnemyState::None;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	EBasicEnemyBehaviour Behaviour = EBasicEnemyBehaviour::None;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<UBasicEnemyConfigData> ConfigData;
	
	UPROPERTY(EditAnywhere, Category = "Parameter")
	FStateTreeReference StateTreeRef;

    UPROPERTY(Transient)
    FStateTreeInstanceData InstanceData;
};

/*
 * Runs a sub state tree asset whose schema class should at least be a super class of the parent's schema
 */
USTRUCT(meta = (DisplayName = "Run SubTree"))
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
