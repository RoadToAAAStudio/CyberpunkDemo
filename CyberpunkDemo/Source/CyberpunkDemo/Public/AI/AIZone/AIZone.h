// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeComponent.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "GameFramework/Actor.h"
#include "AIZone.generated.h"

class UBoxComponent;
class ALocation;
class ABasicEnemy;

UENUM(BlueprintType)
enum class EAIZoneState : uint8
{
	Unaware,
	Combat,
	Alerted,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FAIZoneMapping : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAIZoneState StateEnum = EAIZoneState::Unaware;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerIsSensed, const ABasicEnemyController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerIsForgotten);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatTimerStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatTimerFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlertedTimerStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlertedTimerFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerIsInNoSightConeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIZoneManagerStateChangedSignature, EAIZoneState, SourceState, EAIZoneState, NewState);

UCLASS()
class CYBERPUNKDEMO_API AAIZone : public AActor, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()
	friend class ABasicEnemy;
public:

#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable)
	FOnPlayerIsSensed OnPlayerIsSensedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerIsForgotten OnPlayerIsForgottenDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnCombatTimerStarted OnCombatTimerStartedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnCombatTimerFinished OnCombatTimerFinishedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAlertedTimerStarted OnAlertedTimerStartedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAlertedTimerFinished OnAlertedTimerFinishedDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerIsInNoSightConeSignature OnPlayerIsInNoSightConeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAIZoneManagerStateChangedSignature OnAIZoneManagerStateChangedDelegate;
#pragma endregion

protected:
	
#pragma region SHARED_KNOWLEDGE
	/*
	 * If Player is not null it means all enemies know where the main character is (i.e. Combat State).
	 * This is set if AIZone is in Unaware or Alerted State and some BasicEnemy sees the player
	 * This is unset Combat state is exited
	 */
	UPROPERTY()
	TObjectPtr<AActor> Player;

	/*
	 * The Combat Timer is Set when CurrentState is Combat and the Player is in no BasicEnemy SightCone
	 * This is cleared if the CurrentState is Combat and the Player enters some BasicEnemySightCone
	 */
	UPROPERTY()
	FTimerHandle CombatTimer;

	/*
	 * The Alerted Timer is Set when Alerted is entered
	 * this is cleared if the Current State is Alerted and the Player is seen
	 */
	UPROPERTY()
	FTimerHandle AlertedTimer;

	/*
	 * Increased/Decreased when the player enters/exits a sight cone
	 */
	UPROPERTY()
	int NumberOfSightConesThePlayerIsIn = 0;

	/*
	 * List of Enemies (spawned) in the TriggerBox
	 */
	UPROPERTY()
	TArray<TObjectPtr<ABasicEnemy>> Enemies;

	/*
	 * List of Cover Points (Spawned) in the TriggerBox
	 */
	UPROPERTY()
	TArray<TObjectPtr<ALocation>> CoverLocations;
	
	/*
	 * This reflects AIZone State Tree current state
	 * Transitions:
	 *		Unaware:
	 *			To Combat: if some BasicEnemy sees the player
	 *		Combat:
	 *			To Alerted: if the combat timer expires
	 *		Alerted:
	 *			To Combat: if some BasicEnemy sees the player
	 *			To Unaware: if the alerted timer expires
	 */
	UPROPERTY()
	EAIZoneState CurrentState = EAIZoneState::Unaware;
#pragma endregion
	
#pragma region PERSONAL_COMPONENTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Personal")
	TObjectPtr<UBoxComponent> BoxTrigger;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "Personal")
	TObjectPtr<UStateTreeComponent> StateTree;
#pragma endregion

public:	
	// Sets default values for this actor's properties
	AAIZone();
	
#pragma region SHARED_KNOWLEDGE_GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	const AActor* GetPlayer() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	FTimerHandle GetCombatTimerHandle() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	FTimerHandle GetAlertedTimerHandle() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	int GetNumberOfSightConesThePlayerIsIn() const; 

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	TArray<ABasicEnemy*> GetEnemies() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	TArray<ALocation*> GetCoverLocations() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	EAIZoneState GetCurrentState() const;
#pragma endregion
	
	// StateTree notifications acceptor
	void AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition) override;
	
protected:

#pragma region BLUEPRINT_EVENTS
	// Hook for Derived Blueprints when a StateTree's state change
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerIsSensed"))
	void PlayerIsSensed();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerIsForgotten"))
	void PlayerIsForgotten();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnCombatTimerStarted"))
	void CombatTimerStarted();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnCombatTimerFinished"))
	void CombatTimerFinished();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnAlertedTimerStarted"))
	void AlertedTimerStarted();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnAlertedTimerFinished"))
	void AlertedTimerFinished();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerIsInNoSightCone"))
	void PlayerIsInNoSightCone();
	
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))
	void StateChanged(const EAIZoneState SourceState, const EAIZoneState NewState);
#pragma endregion
	
private:

#pragma region FUNCTIONS_LISTENERS
	// Function listeners
	UFUNCTION()
	void NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	void NotifyPlayerEnteredInSightCone(const ABasicEnemyController* Controller);

	UFUNCTION()
	void NotifyPlayerExitedInSightCone(const ABasicEnemyController* Controller);
	
	UFUNCTION()
	void NotifyPlayerWasSeen(const ABasicEnemyController* Controller);
#pragma endregion 

#pragma region FUNCTIONS_OVERRIDES	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#pragma endregion 
};
