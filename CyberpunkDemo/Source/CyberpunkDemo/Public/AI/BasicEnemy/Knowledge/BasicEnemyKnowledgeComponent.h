// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicEnemyKnowledgeTypes.h"
#include "Components/ActorComponent.h"
#include "BasicEnemyKnowledgeComponent.generated.h"

class UBasicEnemySensorsData;
class UAttributeBar;
class UBasicEnemyPerceptionComponent;
class ABasicEnemyController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams	(FOnSenseToggledSignature,				const UClass*, SenseConfig, bool, Enabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam     (FOnPlayerEnteredSightConeSignature,	const APawn*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam     (FOnPlayerExitedSightConeSignature,		const APawn*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam     (FOnPlayerSeenSignature,				const APawn*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam     (FOnPlayerHiddenSignature,				const APawn*, Owner);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams    (FOnSomethingHeardSignature,            const APawn*, Owner, const FAIStimulus, Stimulus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams    (FOnSoundForgottenSignature,            const APawn*, Owner, const FAIStimulus, Stimulus);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UBasicEnemyKnowledgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnSenseToggledSignature OnSenseToggledDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPlayerEnteredSightConeSignature OnPlayerEnteredSightConeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPlayerExitedSightConeSignature OnPlayerExitedSightConeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPlayerSeenSignature OnPlayerSeenDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnPlayerHiddenSignature OnPlayerHiddenDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnSomethingHeardSignature OnSomethingHeardDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnSoundForgottenSignature OnSoundForgottenDelegate;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAttributeBar> SightBar;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAttributeBar> HearingBar;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "AI | Knowledge")
	FBasicEnemyPersonalKnowledge PersonalKnowledge;

	UBasicEnemyKnowledgeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetUpFromData(const UBasicEnemySensorsData* SensorsConfigData);

	void Initialize(UBasicEnemyPerceptionComponent* PerceptionComponent, ABasicEnemyController* BasicEnemyController);

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	FORCEINLINE bool IsSightEnabled() const;

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	FORCEINLINE bool IsHearingEnabled() const;

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	FORCEINLINE float GetSightBarValue() const;

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	FORCEINLINE float GetHearingBarValue() const;

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	void ToggleSight(bool Enable);

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	void ToggleHearing(bool Enable);

protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSenseToggled"))				
	void SenseToggled(const UClass* SenseConfig, bool Enabled);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerEnteredSightCone"))	
	void PlayerEnteredSightCone();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerExitedSightCone"))	
	void PlayerExitedSightCone();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerSeen"))				
	void PlayerSeen();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerHidden"))				
	void PlayerHidden();

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSomethingHeard"))           
	void SomethingHeard(const FAIStimulus Stimulus);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSoundForgotten"))           
	void SoundForgotten(const FAIStimulus Stimulus);
	
private:
	UPROPERTY()
	TObjectPtr<UBasicEnemyPerceptionComponent> PerceptionComponent;

	bool bIsSightEnabled = true;
	bool bIsHearingEnabled = true;
	float SightBaseIncreaseRate = 1.0f;
	float SightBaseDecreaseRate = 1.0f;
	float SightCrouchMultiplier = 1.0f;
	float SightDistanceMinMultiplier = 1.0f;
	float SightDistanceMaxMultiplier = 1.0f;
	float HearingBaseDecreaseRate = 1.0f;

	FAIStimulus TemporaryHeardStimulus;

	// Functions Listeners
	UFUNCTION()     
	void NotifySightBarFull();

	UFUNCTION()     
	void NotifySightBarEmpty();

	UFUNCTION()     
	void NotifyHearingBarFull();

	UFUNCTION()		
	void NotifyReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus);

	UFUNCTION()		
	void NotifyStateChanged(EBasicEnemyState OldState, EBasicEnemyState NewState);
};