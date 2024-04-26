// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Utility/SplineContainer.h"
#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

class UStateTree;
class AAIZone;
struct FBasicEnemyConfigData;
struct FKnowledgeConfigData;
class USettableStateTreeComponent;
class ABasicEnemyController;

UENUM(BlueprintType, Blueprintable)
enum class EBasicEnemyBehaviour : uint8
{
	None,
	Idle,
	ReturnToSpawnPoint,
	Patrol,
	BlindInvestigation,
	Investigation,
	Shoot,
	QuickMeleeAttack,
	ThrowGrenade,
	MoveToCover,
	ShootFromCover,
	ThrowGrenadeFromCover,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FBasicEnemySupportedBehaviourMapping : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBasicEnemyBehaviour BehaviourEnum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStateTree> BehaviourAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsInterruptible;
};

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY() ABasicEnemyController* BasicEnemyController;
	UPROPERTY() AAIZone* AIZone;
	
#pragma region INPUT_DATA
#pragma region DEFINE_ARCHETYPE
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	TObjectPtr<UDataTable> ConfigData;
#pragma endregion
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	TObjectPtr<ASplineContainer> PatrolSpline;
#pragma endregion

private:

public:
	virtual void BeginPlay() override;

private:
	UFUNCTION()	void NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
	
// public:
// 	UPROPERTY(EditAnywhere)
// 	TObjectPtr<ASplineContainer> PatrolSpline;
//
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	FBasicEnemyActuatorsData ActuatorsData;
// 	
// protected:
//
// #pragma region DECISIONMAKING
// 	UPROPERTY(EditAnywhere, Category = "DecisionMaking")
// 	float MinDistanceToShoot = 600.0f;
//
// 	UPROPERTY(EditAnywhere, Category = "DecisionMaking")
// 	float MaxDistanceToShoot = 1500.0f;
//
// 	UPROPERTY(EditAnywhere, Category = "DecisionMaking")
// 	float MaxDistanceToQuickMeleeAttack = 200.0f;
//
// 	/*
// 	 * All possible behaviour for this enemy
// 	 */
// 	UPROPERTY()
// 	TMap<EBasicEnemyBehaviour, UStateTree*> SupportedBehaviours;
//
// 	UPROPERTY()
// 	EBasicEnemyBehaviour ChosenBehaviour;
// 	
// 	UPROPERTY()
// 	TObjectPtr<ABasicEnemyController> BasicEnemyController;
// #pragma endregion
//
// #pragma region ACTUATION
// 	UPROPERTY(EditAnywhere, Category = "Actuation")
// 	float WaitingTimeAtPatrolStart = 5.0f;
//
// 	UPROPERTY(EditAnywhere, Category = "Actuation")
// 	float WaitingTimeAtPatrolEnd = 5.0f;
// #pragma endregion 
// 	
// #pragma region PERSONAL_COMPONENTS	
// 	UPROPERTY(EditAnywhere, Instanced, Category = "DecisionMaking")
// 	TObjectPtr<UStateTreeComponent> StateMachine;
// 	
// 	UPROPERTY(EditAnywhere, Category = "DecisionMaking")
// 	TObjectPtr<UDataTable> SupportedBehavioursDataTable;
//
// 	UPROPERTY(VisibleAnywhere, Category = "Actuation")
// 	TObjectPtr<USettableStateTreeComponent> CurrentBehaviour;
//
// #pragma endregion 
//
// public:
// 	// Sets default values for this character's properties
// 	ABasicEnemy();
//
// #pragma region DECISIONMAKING_GETTERS
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | Knowledge")
// 	ABasicEnemyController* GetBasicEnemyController() const;
// 	
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | DecisionMaking | Behaviour")
// 	TSet<EBasicEnemyBehaviour> GetSupportedBehaviours() const;
// 	
// 	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personal | DecisionMaking | Behaviour")
// 	EBasicEnemyBehaviour GetCurrentChosenBehaviour() const;
// 	
// #pragma endregion
// 	
// private:
// 	void SelectBehaviour();
// 	
// #pragma region FUNCTIONS_LISTENERS
// 	// Function listeners
// 	UFUNCTION()
// 	void NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
// 	
// 	UFUNCTION()
// 	void NotifyPlayerWasSeen(const ABasicEnemyController* NotifierController);
//
// 	UFUNCTION()
// 	void NotifyCombatTimerFinished();
//
// 	UFUNCTION()
// 	void NotifyAlertedTimerFinished();
//
// 	// UFUNCTION()
// 	// void NotifyGoalGenerated(const TSet<EBasicEnemyGoal> NewGoals, const TSet<EBasicEnemyGoal> RemovedGoals);
// 	
// #pragma endregion 
//
// #pragma region FUNCTIONS_OVERRIDES
// public:	
// 	// Called every frame
// 	virtual void Tick(float DeltaTime) override;
//
// 	// Called to bind functionality to input
// 	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//
// protected:
// 	// Called when the game starts or when spawned
// 	virtual void BeginPlay() override;
// #pragma endregion 
//};
