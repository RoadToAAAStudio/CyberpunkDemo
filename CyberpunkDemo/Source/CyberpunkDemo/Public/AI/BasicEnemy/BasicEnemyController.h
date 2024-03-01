// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/Sensing/AttributeBar.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayTagContainer.h"
#include "BasicEnemyController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnteredInSightCone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerExitedFromSightCone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerSeenSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSightSenseToogleSignature, const bool, Enabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSomethingWasHeardSignature, const FAIStimulus, Stimulus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHearingSenseToogleSignature, const bool, Enabled);

/**
 * Basic Enemy Controller
 * It encapsulate data and delegates for AI sensors
 */
UCLASS()
class CYBERPUNKDEMO_API ABasicEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	// Dependencies with Configs and Components
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeBar> SightBar;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeBar> HearingBar;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnPlayerEnteredInSightCone OnPlayerEnteredInSightConeDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerExitedFromSightCone OnPlayerExitedFromSightConeDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnPlayerSeenSignature OnPlayerSeenDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnSightSenseToogleSignature OnSightSenseToggledDelegate;
	UPROPERTY(BlueprintAssignable)
    FOnSomethingWasHeardSignature OnSomethingWasHeardDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnHearingSenseToogleSignature OnHearingSenseToggledDelegate;
	
	// Info to be polled
	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer GameplayTagsContainer;

	// Config variables
	// Rate for filling the sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightIncreaseRate = 0.25f;

	// Rate for emptying the sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightDecreaseRate = 0.25f;

	// Rate for emptying the hearing bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HearingDecreaseRate = 0.25f;

private:
	FAIStimulus CurrentHeardStimulus; 
	
public:
	explicit ABasicEnemyController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void EnableSightSense(bool Enable);

	UFUNCTION(BlueprintCallable)
	void EnableHearingSense(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSightEnabled();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsHearingEnabled();

protected:

	// Derived Blueprints hooks
	/*Blueprint implementable event called when the player enter in the enemy*/
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerEnteredInSightCone"))
	void PlayerEnteredInSightCone();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerExitedFromSightCone"))
	void PlayerExitedFromSightCone();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerSeen"))
	void PlayerSeen();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSomethingWasHeard"))
	void SomethingWasHeard(const FAIStimulus Stimulus);

private:
	void SetupPerceptionSystem();
	
	UFUNCTION()
	void ReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus);

	UFUNCTION()
	void SightBarFull();
	
	UFUNCTION()
	void HearingBarFull();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* PossessedPawn) override;
};
