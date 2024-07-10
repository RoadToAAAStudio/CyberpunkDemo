// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BasicEnemy/Knowledge/KnowledgeTypes.h"
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
	None,
	Unaware,
	Combat,
	Alerted,
	Max UMETA(Hidden)
};

USTRUCT(Blueprintable)
struct FBasicEnemySharedKnowledge
{
	GENERATED_BODY()
	
	/*
	 * If Player is not null it means all enemies know where the main character is (i.e. Combat State).
	 * This is set if AIZone is in Unaware or Alerted State and some BasicEnemy sees the player
	 * This is unset Combat state is exited
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FSettablePawn Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FSettableVector PlayerLocation;

	/*
	 * The Combat Timer is Set when CurrentState is Combat and the Player is in no BasicEnemy SightCone
	 * This is cleared if the CurrentState is Combat and the Player enters some BasicEnemySightCone
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FSettableTimerHandle CombatTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FSettableFloat CombatTimerDuration;


	/*
	 * The Alerted Timer is Set when Alerted is entered
	 * this is cleared if the Current State is Alerted and the Player is seen
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FSettableTimerHandle AlertedTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FSettableFloat AlertedTimerDuration;
	
	/*
	 * Increased/Decreased when the player enters/exits a sight cone
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FSettableInt NumberOfSightConesThePlayerIsIn;

	/*
	 * List of Enemies (spawned) in the TriggerBox
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	TArray<TObjectPtr<ABasicEnemy>> Enemies;

	/*
	 * List of Cover Points (Spawned) in the TriggerBox
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	TMap<FVector, TObjectPtr<ALocation>> CoverPerLocations;
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	EAIZoneState AIZoneState;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam		(FOnPlayerSensedSignature,				const APawn*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE              (FOnPlayerForgottenSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE              (FOnCombatTimerStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE              (FOnCombatTimerFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE              (FOnAlertedTimerStartedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE              (FOnAlertedTimerFinishedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE              (FOnPlayerInNoSightConeSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams	(FOnAIZoneManagerStateChangedSignature,	EAIZoneState, SourceState, EAIZoneState, NewState);

UCLASS()
class CYBERPUNKDEMO_API AAIZone : public AActor, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()

public:
#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable)	FOnPlayerSensedSignature				OnPlayerIsSensedDelegate;
	UPROPERTY(BlueprintAssignable)	FOnPlayerForgottenSignature				OnPlayerIsForgottenDelegate;
	UPROPERTY(BlueprintAssignable)	FOnCombatTimerStartedSignature			OnCombatTimerStartedDelegate;
	UPROPERTY(BlueprintAssignable)	FOnCombatTimerFinishedSignature         OnCombatTimerFinishedDelegate;
	UPROPERTY(BlueprintAssignable)	FOnAlertedTimerStartedSignature         OnAlertedTimerStartedDelegate;
	UPROPERTY(BlueprintAssignable)	FOnAlertedTimerFinishedSignature		OnAlertedTimerFinishedDelegate;
	UPROPERTY(BlueprintAssignable)	FOnPlayerInNoSightConeSignature			OnPlayerIsInNoSightConeDelegate;
	UPROPERTY(BlueprintAssignable)	FOnAIZoneManagerStateChangedSignature	OnAIZoneManagerStateChangedDelegate;
#pragma endregion

#pragma region SETTINGS
	UPROPERTY(EditDefaultsOnly) float CombatTimerDuration;
	UPROPERTY(EditDefaultsOnly) float AlertedTimerDuration;
#pragma endregion
	
#pragma region PERSONAL_COMPONENTS
	UPROPERTY(BlueprintReadWrite) TObjectPtr<UBoxComponent>		BoxTrigger;
	UPROPERTY(BlueprintReadWrite) TObjectPtr<UStateTreeComponent>	StateMachine;
#pragma endregion
	
	UPROPERTY(BlueprintReadWrite) FBasicEnemySharedKnowledge SharedKnowledge;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bDebugEnemies = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bDebugCovers = false;

	AAIZone();
	virtual void Tick(float DeltaSeconds) override;

#pragma region INTERFACE_METHODS
	void GetChangeOfState_Implementation(const FName& SourceStateName, const FName& NextStateName) override;
#pragma endregion

	
protected:
	virtual void BeginPlay() override;
	
#pragma region BLUEPRINT_EVENTS
	// Hook for Derived Blueprints when a StateTree's state change
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerSensed"))			void PlayerSensed();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerForgotten"))		void PlayerForgotten();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnCombatTimerStarted"))		void CombatTimerStarted();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnCombatTimerFinished"))	void CombatTimerFinished();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnAlertedTimerStarted"))	void AlertedTimerStarted();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnAlertedTimerFinished"))	void AlertedTimerFinished();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerInNoSightCone"))	void PlayerInNoSightCone();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))			void StateChanged(const EAIZoneState SourceState, const EAIZoneState NewState);
#pragma endregion

private:
	void RegisterActors();
	void DebugEnemies() const;
	void DebugCovers() const;
	
#pragma region FUNCTIONS_LISTENERS
	// Function listeners
	UFUNCTION()	void NotifyPlayerEnteredInSightCone(const APawn* PawnOwner);
	UFUNCTION()	void NotifyPlayerExitedInSightCone(const APawn* PawnOwner);
	UFUNCTION()	void NotifyPlayerWasSeen(const APawn* PawnOwner);
#pragma endregion
};
