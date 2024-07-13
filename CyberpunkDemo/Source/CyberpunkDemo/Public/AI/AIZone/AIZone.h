// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BasicEnemy/Knowledge/KnowledgeTypes.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "GameFramework/Actor.h"
#include "AIZone.generated.h"

class UBoxComponent;
class ALocation;
class ABasicEnemy;
class UStateTreeComponent;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FSettablePawn Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FSettableVector PlayerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FSettableTimerHandle CombatTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FSettableFloat CombatTimerDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FSettableTimerHandle AlertedTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FSettableFloat AlertedTimerDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	FSettableInt NumberOfSightConesThePlayerIsIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TArray<TObjectPtr<ABasicEnemy>> Enemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	TMap<FVector, TObjectPtr<ALocation>> CoverPerLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	EAIZoneState AIZoneState;
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
	UPROPERTY(BlueprintAssignable, Category = Delegates)	FOnPlayerSensedSignature				OnPlayerIsSensedDelegate;
	UPROPERTY(BlueprintAssignable, Category = Delegates)	FOnPlayerForgottenSignature				OnPlayerIsForgottenDelegate;
	UPROPERTY(BlueprintAssignable, Category = Delegates)	FOnCombatTimerStartedSignature			OnCombatTimerStartedDelegate;
	UPROPERTY(BlueprintAssignable, Category = Delegates)	FOnCombatTimerFinishedSignature         OnCombatTimerFinishedDelegate;
	UPROPERTY(BlueprintAssignable, Category = Delegates)	FOnAlertedTimerStartedSignature         OnAlertedTimerStartedDelegate;
	UPROPERTY(BlueprintAssignable, Category = Delegates)	FOnAlertedTimerFinishedSignature		OnAlertedTimerFinishedDelegate;
	UPROPERTY(BlueprintAssignable, Category = Delegates)	FOnPlayerInNoSightConeSignature			OnPlayerIsInNoSightConeDelegate;
	UPROPERTY(BlueprintAssignable, Category = Delegates)	FOnAIZoneManagerStateChangedSignature	OnAIZoneManagerStateChangedDelegate;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	bool bDebugSharedKnowledge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	bool bDebugEnemies = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	bool bDebugCovers = false;

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

	void DebugSharedKnowledge() const;
	void DebugEnemies() const;
	void DebugCovers() const;
	
#pragma region FUNCTIONS_LISTENERS
	// Function listeners
	UFUNCTION()	void NotifyPlayerEnteredInSightCone(const APawn* PawnOwner);
	UFUNCTION()	void NotifyPlayerExitedInSightCone(const APawn* PawnOwner);
	UFUNCTION()	void NotifyPlayerWasSeen(const APawn* PawnOwner);
#pragma endregion
};
