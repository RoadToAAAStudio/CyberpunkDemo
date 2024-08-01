// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIZoneKnowledgeTypes.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "GameFramework/Actor.h"
#include "AIZone.generated.h"

class UBoxComponent;
class ALocation;
class ABasicEnemy;
class UStateTreeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam		(FOnPlayerSensedSignature, const APawn*, Owner);
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
	UPROPERTY(BlueprintAssignable, Category = "Delegates")	
	FOnPlayerSensedSignature OnPlayerIsSensedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")	
	FOnPlayerForgottenSignature	OnPlayerIsForgottenDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")	
	FOnCombatTimerStartedSignature OnCombatTimerStartedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")	
	FOnCombatTimerFinishedSignature OnCombatTimerFinishedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")	
	FOnAlertedTimerStartedSignature OnAlertedTimerStartedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")	
	FOnAlertedTimerFinishedSignature OnAlertedTimerFinishedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")	
	FOnPlayerInNoSightConeSignature OnPlayerIsInNoSightConeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")	
	FOnAIZoneManagerStateChangedSignature OnAIZoneManagerStateChangedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI | Configuration")
	float CombatTimerDuration = 4.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI | Configuration")
	float AlertedTimerDuration = 4.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI | Configuration")
	TArray<TObjectPtr<ABasicEnemy>> Enemies;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI | Configuration")
	TArray<TObjectPtr<ALocation>> Covers;
	
	UPROPERTY(BlueprintReadWrite, Category = "AI | Knowledge")
	FBasicEnemySharedKnowledge SharedKnowledge;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> BoxTrigger;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStateTreeComponent>	StateMachine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugEnemies = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugCovers = false;

	AAIZone();

	virtual void Tick(float DeltaSeconds) override;

	void GetChangeOfState_Implementation(const FName& SourceStateName, const FName& NextStateName) override;
	
protected:
	virtual void BeginPlay() override;
	
	// Hooks for Derived Blueprints when a StateTree's state change
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerSensed"))			
	void PlayerSensed();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerForgotten"))		
	void PlayerForgotten();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnCombatTimerStarted"))		
	void CombatTimerStarted();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnCombatTimerFinished"))	
	void CombatTimerFinished();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnAlertedTimerStarted"))	
	void AlertedTimerStarted();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnAlertedTimerFinished"))	
	void AlertedTimerFinished();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnPlayerInNoSightCone"))	
	void PlayerInNoSightCone();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))			
	void StateChanged(const EAIZoneState SourceState, const EAIZoneState NewState);

private:
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	void DebugEnemies() const;

	void DebugCovers() const;
#endif

	// Function listeners
	UFUNCTION()	
	void NotifyPlayerEnteredInSightCone(const APawn* PawnOwner);

	UFUNCTION()	
	void NotifyPlayerExitedInSightCone(const APawn* PawnOwner);

	UFUNCTION()	
	void NotifyPlayerWasSeen(const APawn* PawnOwner);
};