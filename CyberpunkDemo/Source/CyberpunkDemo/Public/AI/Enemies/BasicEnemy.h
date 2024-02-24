// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	EBasicEnemyState StateEnum;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBasicEnemyStateChangedSignature, EBasicEnemyState, SourceState, EBasicEnemyState, NewState);

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemy : public ACharacter, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Instanced, Category = "AI")
	TObjectPtr<UStateTreeComponent> StateTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Tags")
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(BlueprintAssignable)
	FOnBasicEnemyStateChangedSignature OnBasicEnemyStateChangedDelegate;

	UPROPERTY(BlueprintReadOnly)
	EBasicEnemyState CurrentState;
	
public:
	// Sets default values for this character's properties
	ABasicEnemy();

	// StateTree notifications acceptor
	void AcceptStateTreeNotification_Implementation(const UStateTree* StateTree, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition) override;

protected:
	// Hook for Derived Blueprints when a StateTree's state change
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "OnStateChanged"))
	void StateChanged(const EBasicEnemyState SourceState, const EBasicEnemyState NewState);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
