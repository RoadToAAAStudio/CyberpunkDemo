// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/Sensing/AttributeBar.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayTagContainer.h"
#include "BasicEnemyController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnteredInSightCone, const ABasicEnemyController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerExitedFromSightCone, const ABasicEnemyController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSeenSignature, const ABasicEnemyController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSightSenseToogleSignature, const bool, Enabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHearingSenseToogleSignature, const bool, Enabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSomethingWasHeardSignature, const FAIStimulus, Stimulus);

/**
 * Basic Enemy Controller
 * It encapsulate data and delegates for AI sensors
 */
UCLASS()
class CYBERPUNKDEMO_API ABasicEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	
#pragma region COMPONENTS
	// Dependencies with Configs and Components
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeBar> SightBar;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeBar> HearingBar;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;
#pragma endregion
	
#pragma region DELEGATES
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
#pragma endregion
	
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

protected:
	// Info to be polled
	UPROPERTY()
	FGameplayTagContainer GameplayTagsContainer;
	
private:
	FAIStimulus CurrentHeardStimulus; 
	
public:
	explicit ABasicEnemyController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FGameplayTagContainer GetGameplayTagContainer() const;
	
	UFUNCTION(BlueprintCallable)
	void EnableSightSense(bool Enable);

	UFUNCTION(BlueprintCallable)
	void EnableHearingSense(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSightEnabled();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsHearingEnabled();

protected:

#pragma region BLUEPRINT_EVENTS
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
#pragma endregion
	
private:
	void SetupPerceptionSystem();

#pragma region FUNCTIONS_LISTENERS
	UFUNCTION()
	void NotifyReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus);

	UFUNCTION()
	void NotifySightBarFull();
	
	UFUNCTION()
	void NotifyHearingBarFull();
#pragma endregion 

#pragma region FUNCTIONS_OVERRIDES
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* PossessedPawn) override;
#pragma endregion 
};
