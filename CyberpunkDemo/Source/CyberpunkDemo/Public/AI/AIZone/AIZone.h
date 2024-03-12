// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "GameFramework/Actor.h"
#include "AIZone.generated.h"

class ALocation;

UENUM(BlueprintType)
enum class EAIZoneManagerState : uint8
{
	Unaware,
	Combat,
	Alerted,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FAIZoneManagerMapping : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAIZoneManagerState StateEnum = EAIZoneManagerState::Unaware;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerIsKnown);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerIsForgotten);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatTimerStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatTimerFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlertedTimerStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAlertedTimerFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIZoneManagerStateChangedSignature, EAIZoneManagerState, SourceState, EAIZoneManagerState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerIsInNoSightConeSignature);

UCLASS()
class CYBERPUNKDEMO_API AAIZone : public AActor, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAIZoneManagerStateChangedSignature OnAIZoneManagerStateChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerIsInNoSightConeSignature OnPlayerIsInNoSightConeDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Shared | Knowledge")
	TObjectPtr<AActor> Player;
	
	UPROPERTY(BlueprintReadOnly, Category = "Shared | Knowledge")
	FTimerHandle CombatTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Shared | Knowledge")
	FTimerHandle AlertedTimer;

	UPROPERTY(BlueprintReadOnly, Category = "Shared | Knowledge")
	int NumberOfSightConesThePlayerIsIn = 0;
	
	UPROPERTY(BlueprintReadOnly, Category = "Shared | Knowledge")
	TArray<TObjectPtr<ABasicEnemy>> Enemies;

	UPROPERTY(BlueprintReadOnly, Category = "Shared | Knowledge")
	TArray<TObjectPtr<ALocation>> CoverLocations;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "Personal")
	TObjectPtr<UStateTreeComponent> StateTree;

	UPROPERTY(BlueprintReadOnly, Category= "Personal | Knowledge")
	EAIZoneManagerState CurrentState = EAIZoneManagerState::Unaware;

public:	
	// Sets default values for this actor's properties
	AAIZone();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	const AActor* GetPlayer();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	FTimerHandle GetCombatTimerHandle();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	FTimerHandle GetAlertedimerHandle();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	int GetNumberOfSightConesThePlayerIsIn(); 

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	const TArray<ABasicEnemy*> GetEnemies();
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Shared | Knowledge")
	const TArray<ALocation*> GetCoverLocations();
	
	// StateTree notifications acceptor
	void AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition) override;
	
protected:
	// Hook for Derived Blueprints when a StateTree's state change
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))
	void StateChanged(const EAIZoneManagerState SourceState, const EAIZoneManagerState NewState);

private:
	// Function listeners
	UFUNCTION()
	void NotifyPlayerEnteredInSightCone();

	UFUNCTION()
	void NotifyPlayerExitedInSightCone();
	
	UFUNCTION()
	void NotifyPlayerWasSeen();
	
	UFUNCTION()
	void NotifyEnemyChangedState(EBasicEnemyState SourceState, EBasicEnemyState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
