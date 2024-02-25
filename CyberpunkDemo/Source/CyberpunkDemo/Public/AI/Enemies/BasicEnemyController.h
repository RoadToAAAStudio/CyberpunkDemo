// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/AttributeBar.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayTagContainer.h"
#include "BasicEnemyController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnteredInSightCone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerExitedFromSightCone);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeardSomethingSignature, const FAIStimulus, Stimulus);

/**
 * 
 */
UCLASS()
class CYBERPUNKDEMO_API ABasicEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeBar> SightBar;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeBar> HearingBar;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerEnteredInSightCone OnPlayerEnteredInSightCone;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerExitedFromSightCone OnPlayerExitedFromSightCone;
	
	UPROPERTY(BlueprintAssignable)
    FOnHeardSomethingSignature OnHeardSomethingDelegate;

	// Rate for filling the sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightIncreaseRate = 0.25f;

	// Rate for empting the sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightDecreaseRate = 0.25f;

	// Rate for empting the hearing bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HearingDecreaseRate = 0.25f;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTagContainer GameplayTagsContainer;
	
public:
	explicit ABasicEnemyController(const FObjectInitializer& ObjectInitializer);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:

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
	void ReceivePlayerSeen();

protected:
	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* PossessedPawn) override;
};
