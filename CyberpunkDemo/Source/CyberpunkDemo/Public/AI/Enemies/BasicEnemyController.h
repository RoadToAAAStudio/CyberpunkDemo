// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BasicEnemyController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerSeenSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLostSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeardSomethingSignature, const FAIStimulus, Stimulus);

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API ABasicEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerSeenSignature OnPlayerSeenDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerLostSignature OnPlayerLostDelegate;
	
	UPROPERTY(BlueprintAssignable)
    FOnHeardSomethingSignature OnHeardSomethingDelegate;
public:
	explicit ABasicEnemyController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* PossessedPawn) override;

	/*Blueprint implementable event called when the player enter in the enemy*/
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerSeen"))
	void PlayerSeen();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerLost"))
	void PlayerLost();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSomethingWasHeard"))
	void SomethingWasHeard(const FAIStimulus Stimulus);

private:
	void SetupPerceptionSystem();
	
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus);
};
