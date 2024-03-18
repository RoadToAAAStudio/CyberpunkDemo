// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEnemyController.h"
#include "AI/AIZone/AIZone.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "AI/Utility/SplineContainer.h"
#include "Components/StateTreeComponent.h"
#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

UENUM(BlueprintType)
enum class EBasicEnemyGoal : uint8
{
	None,
	Idle,
	Investigation,
	Patrol,
	Combat,
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EBasicEnemyBehaviour : uint8
{
	None,
	Investigation,
	Patrol,
	Combat,
	Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EBasicEnemyState : uint8
{
	Unaware,
	Combat,
	Alerted,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBasicEnemyStateMapping : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBasicEnemyState StateEnum = EBasicEnemyState::Unaware;
};

USTRUCT(BlueprintType)
struct FBasicEnemySupportedBehaviourMapping : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<EBasicEnemyBehaviour> BehavioursEnum;
};

USTRUCT(BlueprintType)
struct FSetBasicEnemyBehaviourWrapper
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<EBasicEnemyBehaviour> BehavioursEnum;
};

USTRUCT(BlueprintType)
struct FBasicEnemyBehavioursPerGoalMapping : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EBasicEnemyGoal, FSetBasicEnemyBehaviourWrapper> BehaviourEnum;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBasicEnemyStateChangedSignature, EBasicEnemyState, SourceState, EBasicEnemyState, NewState);

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemy : public ACharacter, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()
	friend struct FStateTreeBasicEnemyEvaluator;
	friend struct FStateTreeSelectBehaviourTask;
	
public:
	
#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable)
	FOnBasicEnemyStateChangedSignature OnBasicEnemyStateChangedDelegate;
#pragma endregion

protected:

#pragma region PERSONAL_KNOWLEDGE
	UPROPERTY()
	FVector SpawnLocation;
	
	UPROPERTY()
	FGameplayTagContainer GameplayTagContainer;
	
	/*
	 * This reflects BasicEnemy State Tree current state
	 * Transitions:
	 *		Unaware:
	 *			To Combat: if the BasicEnemy sees the player or is Sensed by someone else
	 *		Combat:
	 *			To Alerted: if the combat timer expires
	 *		Alerted:
	 *			To Combat: if the BasicEnemy sees the player or is Sensed by someone else
	 *			To Unaware: if the alerted timer expires
	 */
	UPROPERTY()
	EBasicEnemyState CurrentState = EBasicEnemyState::Unaware;

	UPROPERTY()
	TSet<EBasicEnemyGoal> CurrentGeneratedGoals;

	UPROPERTY()
	TSet<EBasicEnemyBehaviour> SupportedBehaviours;

	UPROPERTY()
	EBasicEnemyBehaviour CurrentChosenBehaviour;
	
	UPROPERTY()
	TObjectPtr<ABasicEnemyController> BasicEnemyController;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ASplineContainer> PatrolSpline;
#pragma endregion

#pragma region SHARED_KNOWLEDGE
	UPROPERTY()
	TObjectPtr<AAIZone> SharedKnowledge;
#pragma endregion 

#pragma region PERSONAL_COMPONENTS	
	UPROPERTY(EditAnywhere, Instanced, Category = "DecisionMaking")
	TObjectPtr<UStateTreeComponent> StateTree;

	UPROPERTY(EditAnywhere, Category = "DecisionMaking")
	TObjectPtr<UDataTable> SupportedBehavioursDataTable;
#pragma endregion 

public:
	// Sets default values for this character's properties
	ABasicEnemy();
	
#pragma region KNOWLEDGE_GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	const AAIZone* GetSharedKnowledge() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal |Knowldge ")
	FVector GetSpawnLocation() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
	const ASplineContainer* GetPatrolSpline() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
	FGameplayTagContainer GetGameplayTagContainer() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
	EBasicEnemyState GetCurrentState() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge | Goal")
	TSet<EBasicEnemyGoal> GetCurrentGeneratedGoals() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge | Behaviour")
	TSet<EBasicEnemyBehaviour> GetSupportedBehaviours() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge | Behaviour")
	EBasicEnemyBehaviour GetCurrentChosenBehaviour() const;
#pragma endregion
	
	// StateTree notifications acceptor
	void AcceptStateTreeNotification_Implementation(const FName& SourceStateName, const FName& CurrentStateName) override;
	
protected:
	
#pragma region BLUEPRINT_EVENTS
	// Hook for Derived Blueprints when a StateTree's state change
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))
	void StateChanged(const EBasicEnemyState SourceState, const EBasicEnemyState NewState);
#pragma endregion 

private:

#pragma region FUNCTIONS_LISTENERS
	// Function listeners
	UFUNCTION()
	void NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void NotifyPlayerWasSeen(const ABasicEnemyController* NotifierController);

	UFUNCTION()
	void NotifyCombatTimerFinished();

	UFUNCTION()
	void NotifyAlertedTimerFinished();
	
#pragma endregion 

#pragma region FUNCTIONS_OVERRIDES
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#pragma endregion 
};
