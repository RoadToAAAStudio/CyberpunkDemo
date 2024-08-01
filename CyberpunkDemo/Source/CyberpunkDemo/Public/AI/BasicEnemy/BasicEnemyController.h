// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Knowledge/BasicEnemyKnowledgeTypes.h"
#include "AI/Utility/IStateTreeNotificationsAcceptor.h"
#include "BasicEnemyController.generated.h"

class USettableStateTreeComponent;
class UBasicEnemyKnowledgeComponent;
class UBasicEnemyPerceptionComponent;
class ABasicEnemy;
class AAIZone;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams (FOnStateChangedSignature, EBasicEnemyState, SourceState, EBasicEnemyState, NextState);

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemyController : public AAIController, public IStateTreeNotificationsAcceptor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnStateChangedSignature OnBasicEnemyStateChangedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<ABasicEnemy> BasicEnemy;
	
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBasicEnemyPerceptionComponent> SenseComponent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBasicEnemyKnowledgeComponent> KnowledgeComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<USettableStateTreeComponent> StateMachine;
	
	ABasicEnemyController();

	void Initialize(ABasicEnemy* BasicEnemy);

	void RegisterAIZone(AAIZone* NewAIZone);
	
	void GetChangeOfState_Implementation(const FName& SourceStateName, const FName& NextStateName) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnStateChanged"))	
	void StateChanged(EBasicEnemyState SourceState, EBasicEnemyState NextState);

private:
	UFUNCTION() 
	void NotifyPlayerWasSeen(const APawn* Notifier);

	UFUNCTION()
	void NotifyPlayerWasSeenAcrossNetwork(const APawn* Notifier);

	UFUNCTION() 
	void NotifyCombatTimerFinished();

	UFUNCTION() 
	void NotifyAlertedTimerFinished();

	UFUNCTION() 
	void NotifySomethingWasHeard(const APawn* Notifier, const FAIStimulus Stimulus);

	UFUNCTION() 
	void NotifySoundForgotten(const APawn* Notifier, const FAIStimulus Stimulus);
};