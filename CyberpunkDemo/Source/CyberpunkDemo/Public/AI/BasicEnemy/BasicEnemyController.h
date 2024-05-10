// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "AI/AIZone/AIZone.h"
#include "BasicEnemyController.generated.h"

class USettableStateTreeComponent;
class UBasicEnemyKnowledgeComponent;
class UBasicEnemyPerceptionComponent;
enum class EBasicEnemyState : uint8;

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnteredInSightCone, const ABasicEnemyController*, Controller);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerExitedFromSightCone, const ABasicEnemyController*, Controller);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSeenSignature, const ABasicEnemyController*, Controller);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSomethingWasHeardSignature, const FAIStimulus, Stimulus);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSightSenseToggleSignature, const bool, Enabled);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHearingSenseToggleSignature, const bool, Enabled);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGoalsChanged, const TSet<EBasicEnemyGoalType>&, NewGoals, const TSet<EBasicEnemyGoalType>&, RemovedGoal);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBasicEnemyStateChangedSignature, EBasicEnemyState, SourceState, EBasicEnemyState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams	(FOnStateChangedSignature, EBasicEnemyState, SourceState, EBasicEnemyState, NextState);

/**
 * Basic Enemy Controller
 * It encapsulates AI functionality and serves as interface for the extern
 */
UCLASS()
class CYBERPUNKDEMO_API ABasicEnemyController : public AAIController, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	ABasicEnemy* BasicEnemy = nullptr;
#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable) FOnStateChangedSignature OnBasicEnemyStateChangedDelegate;
#pragma endregion
	
#pragma region AI_COMPONENTS
	UPROPERTY(BlueprintReadOnly) TObjectPtr<UBasicEnemyPerceptionComponent> SenseComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<UBasicEnemyKnowledgeComponent> KnowledgeComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TObjectPtr<USettableStateTreeComponent> StateMachine;
#pragma endregion
private:
#pragma region INJECTED_DEPENDENCIES
#pragma endregion
public:
	explicit ABasicEnemyController(const FObjectInitializer& ObjectInitializer);
	void Initialize(ABasicEnemy* BasicEnemy);
	
#pragma region INTERFACE_METHODS
	void GetChangeOfState_Implementation(const FName& SourceStateName, const FName& NextStateName) override;
#pragma endregion

protected:
#pragma region BLUEPRINT_EVENT
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnStateChanged"))	void StateChanged(EBasicEnemyState SourceState, EBasicEnemyState NextState);
#pragma endregion
	
private:
#pragma region EVENT_LISTENERS
	UFUNCTION() void NotifyPlayerWasSeen(const APawn* Notifier);
	UFUNCTION() void NotifyCombatTimerFinished();
	UFUNCTION() void NotifyAlertedTimerFinished();
	UFUNCTION() void NotifySomethingWasHeard(const APawn* Notifier, const FAIStimulus Stimulus);
	UFUNCTION() void NotifySoundForgotten(const APawn* Notifier, const FAIStimulus Stimulus);
#pragma endregion 
};