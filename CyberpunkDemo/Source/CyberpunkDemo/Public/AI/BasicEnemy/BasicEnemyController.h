// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AI/Sensing/AttributeBar.h"
#include "Perception/AIPerceptionTypes.h"
#include "GameplayTagContainer.h"
#include "BasicEnemyController.generated.h"

class AMainCharacter;
class ABasicEnemyController;
class AAIZone;

UENUM(BlueprintType)
enum class EBasicEnemyGoal : uint8
{
	None,
	Patrol,
	Search,
	Reaction,
	Combat,
	Cover,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBasicEnemyPersonalKnowledge
{
	GENERATED_BODY()
	
	UPROPERTY()
	FVector SpawnLocation;
	
	UPROPERTY()
	TObjectPtr<ASplineContainer> PatrolSpline;

	UPROPERTY()
	TObjectPtr<ACharacter> PlayerInSightCone;
	
	UPROPERTY()
	FAIStimulus CurrentHeardStimulus; 
	
	UPROPERTY()
	FGameplayTagContainer Tags;

	UPROPERTY()
	TSet<EBasicEnemyGoal> CurrentGeneratedGoals;
	
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEnteredInSightCone, const ABasicEnemyController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerExitedFromSightCone, const ABasicEnemyController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerSeenSignature, const ABasicEnemyController*, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSomethingWasHeardSignature, const FAIStimulus, Stimulus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSightSenseToogleSignature, const bool, Enabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHearingSenseToogleSignature, const bool, Enabled);

/**
 * Basic Enemy Controller
 * It encapsulates data and delegates for AI sensors, knowledge and goals
 */
UCLASS()
class CYBERPUNKDEMO_API ABasicEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	
#pragma region SENSORS_COMPONENTS
	// Dependencies with Configs and Components
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeBar> SightBar;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAttributeBar> HearingBar;
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

#pragma region SENSORS_CONFIGS	
	// Config variables
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;
	
	// Rate for filling the sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightIncreaseRate = 1.0f;

	// Rate for emptying the sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightDecreaseRate = 1.0f;

	// Min Rate multiplier based on distance for sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightMinMultiplier = 1.0f;
	
	// Max Rate multiplier based on distance for sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightMaxMultiplier = 1.0f;

	// Crouch multiplier for sight bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightCrouchMultiplier = 1.0f;
	
	// Rate for emptying the hearing bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HearingDecreaseRate = 1.0f;
#pragma endregion

#pragma region KNOWLEDGE_GETTERS
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal |Knowldge ")
	FVector GetSpawnLocation() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
	const ASplineContainer* GetPatrolSpline() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
	FGameplayTagContainer GetTags() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge | Goal")
	TSet<EBasicEnemyGoal> GetCurrentGeneratedGoals() const;

#pragma region SHARED_KNOWLEDGE
	UPROPERTY(BlueprintReadOnly, Category = "Shared | Knowledge")
	TObjectPtr<AAIZone> SharedKnowledge;
#pragma endregion

#pragma endregion

	
protected:

#pragma region PERSONAL_KNOWLEDGE
	UPROPERTY()
	FBasicEnemyPersonalKnowledge PersonalKnowledge;
#pragma endregion

public:
	explicit ABasicEnemyController(const FObjectInitializer& ObjectInitializer);

#pragma region SENSORS_CONTROL
	UFUNCTION(BlueprintCallable)
	void EnableSightSense(bool Enable);

	UFUNCTION(BlueprintCallable)
	void EnableHearingSense(bool Enable);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSightEnabled();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsHearingEnabled();
#pragma endregion
	
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

	void SensorsUpdate(float DeltaTime);

	void GoalGeneration();

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
