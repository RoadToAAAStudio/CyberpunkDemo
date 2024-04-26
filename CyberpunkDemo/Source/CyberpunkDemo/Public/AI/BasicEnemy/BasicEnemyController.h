// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "AI/AIZone/AIZone.h"
#include "BasicEnemyController.generated.h"

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
#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable) FOnStateChangedSignature OnBasicEnemyStateChangedDelegate;
#pragma endregion
	
#pragma region AI_COMPONENTS
	UPROPERTY(BlueprintReadOnly) TObjectPtr<UBasicEnemyPerceptionComponent> SenseComponent;
	UPROPERTY(BlueprintReadOnly) TObjectPtr<UBasicEnemyKnowledgeComponent> KnowledgeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UStateTree> StateMachine;
#pragma endregion
private:
#pragma region INJECTED_DEPENDENCIES
	UPROPERTY()	ABasicEnemy* BasicEnemy = nullptr;
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
};
// #pragma region SENSORS_COMPONENTS
// 	// Dependencies with Configs and Components
// 	UPROPERTY(BlueprintReadOnly)
// 	TObjectPtr<UAttributeBar> SightBar;
// 	UPROPERTY(BlueprintReadOnly)
// 	TObjectPtr<UAttributeBar> HearingBar;
// #pragma endregion
// 	
// #pragma region DELEGATES
// 	// Delegates
// 	UPROPERTY(BlueprintAssignable)
// 	FOnPlayerEnteredInSightCone OnPlayerEnteredInSightConeDelegate;
// 	UPROPERTY(BlueprintAssignable)
// 	FOnPlayerExitedFromSightCone OnPlayerExitedFromSightConeDelegate;
// 	UPROPERTY(BlueprintAssignable)
// 	FOnPlayerSeenSignature OnPlayerSeenDelegate;
// 	UPROPERTY(BlueprintAssignable)
// 	FOnSightSenseToogleSignature OnSightSenseToggledDelegate;
// 	UPROPERTY(BlueprintAssignable)
//     FOnSomethingWasHeardSignature OnSomethingWasHeardDelegate;
// 	UPROPERTY(BlueprintAssignable)
// 	FOnHearingSenseToogleSignature OnHearingSenseToggledDelegate;
// 	UPROPERTY(BlueprintAssignable)
// 	FOnGoalsChanged OnGoalsChanged;
// 	UPROPERTY(BlueprintAssignable)
// 	FOnBasicEnemyStateChangedSignature OnBasicEnemyStateChangedDelegate;
// #pragma endregion
//
// #pragma region SENSORS_CONFIGS	
// 	// Config variables
// 	UPROPERTY()
// 	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
// 	UPROPERTY()
// 	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;
// 	
// 	// Rate for filling the sight bar
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float SightIncreaseRate = 1.0f;
//
// 	// Rate for emptying the sight bar
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float SightDecreaseRate = 1.0f;
//
// 	// Min Rate multiplier based on distance for sight bar
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float SightMinMultiplier = 1.0f;
// 	
// 	// Max Rate multiplier based on distance for sight bar
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float SightMaxMultiplier = 1.0f;
//
// 	// Crouch multiplier for sight bar
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float SightCrouchMultiplier = 1.0f;
// 	
// 	// Rate for emptying the hearing bar
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	float HearingDecreaseRate = 1.0f;
// #pragma endregion
//
// protected:
//
// #pragma region KNOWLEDGE
// 	UPROPERTY()
// 	FBasicEnemyPersonalKnowledge PersonalKnowledge;
//
// 	UPROPERTY()
// 	TObjectPtr<FBasicEnemySharedKnowledge> SharedKnowledge;
// #pragma endregion
//
// 	UPROPERTY()
// 	TMap<EBasicEnemyGoalType, UGoalGenerator*> SupportedGoalGenerators; 
// 	
// 	// Link to BasicEnemy
// 	UPROPERTY()
// 	TObjectPtr<ABasicEnemy> BasicEnemy;
// 	
// private:
// 	// Temp variable for a hearing stimulus
// 	FAIStimulus CurrentHeardStimulus;
// 	
// public:
// 	explicit ABasicEnemyController(const FObjectInitializer& ObjectInitializer);
//
// 	// StateTree notifications acceptor
// 	void AcceptStateTreeNotification_Implementation(const FName& SourceStateName, const FName& CurrentStateName) override;
//
// #pragma region KNOWLEDGE_GETTERS
//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal |Knowldge ")
// 	// FVector GetSpawnLocation() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// float GetDistanceFromSpawn() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// const ASplineContainer* GetPatrolSpline() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// const ACharacter* GetPlayerInSightCone() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// float GetDistanceFromPlayer() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// FVector GetSensedLocation() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// bool IsSensedLocationSet() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// FVector GetCoverLocation() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// bool IsCoverLocationSet() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// FGameplayTagContainer GetTags() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// const TMap<EBasicEnemyGoalType, UGoalGenerator*>& GetGeneratedGoals() const;
// 	//
// 	// UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	// EBasicEnemyState GetCurrentState() const;
//
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Knowledge | Personal")
// 	const FBasicEnemyPersonalKnowledge& GetPersonalKnowledge() const;
// 	
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Knowledge | Shared")
// 	const FBasicEnemySharedKnowledge& GetSharedKnowledge() const;
// #pragma endregion
//
// #pragma endregion
//
// #pragma region SENSORS_CONTROL
// 	UFUNCTION(BlueprintCallable)
// 	void EnableSightSense(bool Enable);
//
// 	UFUNCTION(BlueprintCallable)
// 	void EnableHearingSense(bool Enable);
//
// 	UFUNCTION(BlueprintCallable, BlueprintPure)
// 	bool IsSightEnabled();
//
// 	UFUNCTION(BlueprintCallable, BlueprintPure)
// 	bool IsHearingEnabled();
// #pragma endregion
// 	
// protected:
//
// #pragma region BLUEPRINT_EVENTS
// 	// Derived Blueprints hooks
// 	/*Blueprint implementable event called when the player enter in the enemy*/
// 	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerEnteredInSightCone"))
// 	void PlayerEnteredInSightCone();
//
// 	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerExitedFromSightCone"))
// 	void PlayerExitedFromSightCone();
//
// 	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerSeen"))
// 	void PlayerSeen();
//
// 	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSomethingWasHeard"))
// 	void SomethingWasHeard(const FAIStimulus Stimulus);
//
// 	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnGoalsChanged"))
// 	void GoalsChanged(const TSet<EBasicEnemyGoal>& NewGoals, const TSet<EBasicEnemyGoal>& RemovedGoals);
//
// 	// Hook for Derived Blueprints when a StateTree's state change
// 	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))
// 	void StateChanged(const EBasicEnemyState SourceState, const EBasicEnemyState NewState);
// #pragma endregion
// 	
// private:
// 	void SetupPerceptionSystem();
//
// 	void UpdatePersonalKnowledge();
// 	
// 	void SensorsUpdate(float DeltaTime);
//
// 	void GoalGeneration();
//
// #pragma region FUNCTIONS_LISTENERS
// 	UFUNCTION()
// 	void NotifyReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus);
//
// 	UFUNCTION()
// 	void NotifySightBarFull();
// 	
// 	UFUNCTION()
// 	void NotifyHearingBarFull();
// #pragma endregion 
//
// #pragma region FUNCTIONS_OVERRIDES
// public:
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;
//
// protected:
// 	virtual void BeginPlay() override;
//
// 	virtual void OnPossess(APawn* PossessedPawn) override;
// #pragma endregion 
//};
