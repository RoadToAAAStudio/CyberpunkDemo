// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "GameFramework/Actor.h"
#include "AIZone.generated.h"

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIZoneManagerStateChangedSignature, EAIZoneManagerState, SourceState, EAIZoneManagerState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerIsInNoSightConeSignature);

UCLASS()
class CYBERPUNKDEMO_API AAIZoneManager : public AActor, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnAIZoneManagerStateChangedSignature OnAIZoneManagerStateChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerIsInNoSightConeSignature OnPlayerIsInNoSightConeDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category= "Knowledge | Shared")
	EAIZoneManagerState CurrentState = EAIZoneManagerState::Unaware;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadOnly)
	int NumberOfConesPlayerIsIn = 0;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "DecisionMaking")
	TObjectPtr<UStateTreeComponent> StateTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<ABasicEnemy>> Enemies;
	
public:	
	// Sets default values for this actor's properties
	AAIZoneManager();

	// StateTree notifications acceptor
	void AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition) override;

	// Notify children enemies
	UFUNCTION(BlueprintCallable, Category = "Actuation")
	void NotifyChildrenEnemies(const FGameplayTag& GameplayTag);
	
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
