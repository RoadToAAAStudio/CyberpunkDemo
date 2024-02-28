// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IStateTreeNotificationsAcceptor.h"
#include "AI/Enemies/BasicEnemy.h"
#include "GameFramework/Actor.h"
#include "AIZoneManager.generated.h"

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

UCLASS()
class CYBERPUNKDEMO_API AAIZoneManager : public AActor, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	EAIZoneManagerState CurrentState = EAIZoneManagerState::Unaware;

	UPROPERTY(BlueprintAssignable)
	FOnAIZoneManagerStateChangedSignature OnAIZoneManagerStateChangedDelegate;
	
protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "DecisionMaking")
	TObjectPtr<UStateTreeComponent> StateTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<ABasicEnemy>> Enemies;
	
public:	
	// Sets default values for this actor's properties
	AAIZoneManager();

	// StateTree notifications acceptor
	void AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition) override;

protected:
	// Hook for Derived Blueprints when a StateTree's state change
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))
	void StateChanged(const EAIZoneManagerState SourceState, const EAIZoneManagerState NewState);

private:
	// Function listeners
	UFUNCTION()
	void NotifyPlayerWasSeen();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
