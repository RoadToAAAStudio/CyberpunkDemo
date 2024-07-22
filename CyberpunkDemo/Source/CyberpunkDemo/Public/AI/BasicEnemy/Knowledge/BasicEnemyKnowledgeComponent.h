// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BasicEnemy/Knowledge/KnowledgeTypes.h"
#include "Components/ActorComponent.h"
#include "BasicEnemyKnowledgeComponent.generated.h"

class UBasicEnemySensorsData;
class UGoalGenerator;
class UAttributeBar;
class UBasicEnemyPerceptionComponent;
class ABasicEnemyController;
class USplineComponent;
class AMainCharacter;

UENUM(BlueprintType)
enum class EBasicEnemyState : uint8
{
	None,
	Unaware,
	Combat,
	Alerted,
	Max UMETA(Hidden)
};

UENUM(BlueprintType, Blueprintable)
enum class EBasicEnemyBehaviour : uint8
{
	None,
	Idle,
	ReturnToSpawnPoint,
	Patrol,
	BlindInvestigation,
	Investigation,
	MoveToBetterPosition,
	QuickMeleeAttack,
	ThrowGrenade,
	MoveToCover,
	ShootFromCover,
	ThrowGrenadeFromCover,
	Max UMETA(Hidden)
};

USTRUCT(Blueprintable)
struct FBasicEnemyPersonalKnowledge
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettablePawn Agent;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableVector	AgentLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableVector AgentSpawnLocation;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableRotator AgentSpawnRotation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableFloat AgentDistanceFromSpawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	EBasicEnemyState AgentState = EBasicEnemyState::None;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	EBasicEnemyBehaviour AgentBehaviour = EBasicEnemyBehaviour::None;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableSpline PatrolSpline;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	TArray<FVector>	PatrolWaypoints;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableMainCharacter PlayerInSightCone;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableFloat DistanceFromPlayer;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableVector	SensedLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableVector CoverLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite) 
	FSettableVector MoveToLocation;
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
	bool IsSightEnabled() const;

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	bool IsHearingEnabled() const;

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	float GetSightBarValue() const;

	UFUNCTION(BlueprintCallable, Category = "AI | Perception")
	float GetHearingBarValue() const;

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

	bool bSightEnabled = true;
	bool bHearingEnabled = true;
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