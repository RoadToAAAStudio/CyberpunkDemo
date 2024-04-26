// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIZone/AIZone.h"
#include "AI/Knowledge/KnowledgeTypes.h"
#include "Components/ActorComponent.h"
#include "BasicEnemyKnowledgeComponent.generated.h"

class UAttributeBar;
class UBasicEnemyPerceptionComponent;
class USplineComponent;
class AMainCharacter;
class ABasicEnemyController;

UENUM(BlueprintType)
enum class EBasicEnemyState : uint8
{
	None,
	Unaware,
	Combat,
	Alerted,
	Max UMETA(Hidden)
};

USTRUCT(Blueprintable)
struct FBasicEnemyPersonalKnowledge
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite) FSettablePawn					Agent;
	UPROPERTY(BlueprintReadWrite) FSettableVector			    AgentLocation;
	UPROPERTY(BlueprintReadWrite) FSettableVector			    AgentSpawnLocation;
	UPROPERTY(BlueprintReadWrite) FSettableFloat			    AgentDistanceFromSpawn;
	UPROPERTY(BlueprintReadWrite) EBasicEnemyState				AgentState;
	                                        
	UPROPERTY(BlueprintReadWrite) FSettableSpline				PatrolSpline;
	UPROPERTY(BlueprintReadWrite) FSettableMainCharacter		PlayerInSightCone;
	UPROPERTY(BlueprintReadWrite) FSettableFloat			    DistanceFromPlayer;
	UPROPERTY(BlueprintReadWrite) FSettableAIStimulus		    HeardStimulus;
	UPROPERTY(BlueprintReadWrite) FSettableVector			    CoverLocation;
	//UPROPERTY() TSet<EBasicEnemyGoalType>	    GeneratedGoals;
};

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
#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable)	FOnSenseToggledSignature			OnSenseToggledDelegate;
	UPROPERTY(BlueprintAssignable)	FOnPlayerEnteredSightConeSignature	OnPlayerEnteredSightConeDelegate;
	UPROPERTY(BlueprintAssignable)	FOnPlayerExitedSightConeSignature	OnPlayerExitedSightConeDelegate;
	UPROPERTY(BlueprintAssignable)	FOnPlayerSeenSignature				OnPlayerSeenDelegate;
	UPROPERTY(BlueprintAssignable)	FOnPlayerHiddenSignature			OnPlayerHiddenDelegate;
	UPROPERTY(BlueprintAssignable)	FOnSomethingHeardSignature			OnSomethingHeardDelegate;
	UPROPERTY(BlueprintAssignable)	FOnSoundForgottenSignature			OnSoundForgottenDelegate;
#pragma endregion

	UPROPERTY(BlueprintReadOnly)	TObjectPtr<UAttributeBar> SightBar;
	UPROPERTY(BlueprintReadOnly)	TObjectPtr<UAttributeBar> HearingBar;
	
private:
#pragma region INJECTED_DEPENDENCIES
	UPROPERTY()	UBasicEnemyPerceptionComponent* PerceptionComponent = nullptr;
	UPROPERTY() ABasicEnemyController* BasicEnemyController = nullptr;
	const FBasicEnemySharedKnowledge* SharedKnowledge; 
#pragma endregion
	

	FBasicEnemyPersonalKnowledge PersonalKnowledge;

#pragma region SENSORS_CONFIGS
	bool    bSightEnabled			    = true;
	bool    bHearingEnabled		        = true;
	
	float   SightBaseIncreaseRate       = 1.0f;
	float   SightBaseDecreaseRate	    = 1.0f;
	float	SightCrouchMultiplier		= 1.0f;
	float	SightDistanceMinMultiplier	= 1.0f;
	float	SightDistanceMaxMultiplier	= 1.0f;

	float	HearingBaseDecreaseRate		= 1.0f;
#pragma endregion
	
	FAIStimulus TemporaryHeardStimulus;
	
public:
	UBasicEnemyKnowledgeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetUpFromData(const UDataTable* ConfigData);
	void Initialize(UBasicEnemyPerceptionComponent* PerceptionComponent,
					ABasicEnemyController* BasicEnemyController,
					const FBasicEnemySharedKnowledge* SharedKnowledge);
	UFUNCTION(BlueprintCallable) const FBasicEnemyPersonalKnowledge& GetPersonalKnowledge() const;
	UFUNCTION(BlueprintCallable) const FBasicEnemySharedKnowledge& GetSharedKnowledge() const;
	
#pragma region SENSORS_PUBLIC_CONTROLS
	UFUNCTION(BlueprintCallable)    bool IsSightEnabled		() const;
	UFUNCTION(BlueprintCallable)    bool IsHearingEnabled	() const;
	UFUNCTION(BlueprintCallable)    float GetSightBarValue	() const;
	UFUNCTION(BlueprintCallable)    float GetHearingValue	() const;
	UFUNCTION(BlueprintCallable)    void ToggleSight		(bool Enable);
	UFUNCTION(BlueprintCallable)    void ToggleHearing		(bool Enable);
#pragma endregion

protected:
#pragma region BLUEPRINT_EVENT
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSenseToggled"))				void SenseToggled(const UClass* SenseConfig, bool Enabled);
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerEnteredSightCone"))	void PlayerEnteredSightCone();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerExitedSightCone"))	void PlayerExitedSightCone();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerSeen"))				void PlayerSeen();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnPlayerHidden"))				void PlayerHidden();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSomethingHeard"))           void SomethingHeard(const FAIStimulus Stimulus);
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnSoundForgotten"))           void SoundForgotten(const FAIStimulus Stimulus);
#pragma endregion
	
private:
#pragma region EVENT_LISTENERS
	UFUNCTION()     void NotifySightBarFull();
	UFUNCTION()     void NotifySightBarEmpty();
	UFUNCTION()     void NotifyHearingBarFull();
	UFUNCTION()		void NotifyReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus);
	UFUNCTION()		void NotifyStateChanged(EBasicEnemyState OldState, EBasicEnemyState NewState);
#pragma endregion
};