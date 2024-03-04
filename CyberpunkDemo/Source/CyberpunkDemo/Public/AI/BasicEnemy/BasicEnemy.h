// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEnemyController.h"
#include "AI/IStateTreeNotificationsAcceptor.h"
#include "Components/StateTreeComponent.h"
#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBasicEnemyStateChangedSignature, EBasicEnemyState, SourceState, EBasicEnemyState, NewState);

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemy : public ACharacter, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
	FGameplayTagContainer GameplayTagContainer;
	
	UPROPERTY(BlueprintReadOnly)
	EBasicEnemyState CurrentState = EBasicEnemyState::Unaware;
	
	UPROPERTY(BlueprintAssignable)
	FOnBasicEnemyStateChangedSignature OnBasicEnemyStateChangedDelegate;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ABasicEnemyController> BasicEnemyController;

protected:
	UPROPERTY(EditAnywhere, Instanced, Category = "DecisionMaking")
	TObjectPtr<UStateTreeComponent> StateTree;

	UPROPERTY(EditAnywhere, Category = "Actuation")
	TObjectPtr<UBehaviorTree> BTUnaware;
	
	UPROPERTY(EditAnywhere, Category = "Actuation")
	TObjectPtr<UBehaviorTree> BTCombat;
	
	UPROPERTY(EditAnywhere, Category = "Actuation")
	TObjectPtr<UBehaviorTree> BTAlerted;
	
	
public:
	// Sets default values for this character's properties
	ABasicEnemy();

	// StateTree notifications acceptor
	void AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition) override;

	// ReceiveRequests
	void ReceiveTriggerEvent(const FGameplayTag& GameplayTag) const;
	
protected:
	// Hook for Derived Blueprints when a StateTree's state change
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))
	void StateChanged(const EBasicEnemyState SourceState, const EBasicEnemyState NewState);
	
private:
	// Function listeners
	UFUNCTION()
	void NotifyPlayerWasSeen();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
