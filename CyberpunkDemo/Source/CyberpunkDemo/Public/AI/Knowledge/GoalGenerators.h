// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GoalGenerators.generated.h"

class UBasicEnemyKnowledgeComponent;
class USplineComponent;
struct FBasicEnemyPersonalKnowledge;
struct FBasicEnemySharedKnowledge;

UENUM(BlueprintType)
enum class EBasicEnemyGoalType : uint8
{
	None,
	Patrol,
	Search,
	Reaction,
	Combat,
	Cover,
	Max UMETA(Hidden)
};

UCLASS(Abstract, BlueprintType)
class CYBERPUNKDEMO_API UGoalGenerator : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY() bool bIsGenerated = false;
	UPROPERTY() const UBasicEnemyKnowledgeComponent* Knowledge;
public:
	virtual void Initialize(const UBasicEnemyKnowledgeComponent* KnowledgeInput) { Knowledge = KnowledgeInput; };
	UFUNCTION(BlueprintCallable, BlueprintPure) virtual bool CanBeGenerated() const { return false; };
	virtual void Generate() { bIsGenerated = true; };
	virtual void Destroy() { bIsGenerated = false; };
	UFUNCTION(BlueprintCallable, BlueprintPure) virtual EBasicEnemyGoalType GetType() const { return EBasicEnemyGoalType::None; }
};

UCLASS(BlueprintType)
class CYBERPUNKDEMO_API UPatrolGoalGenerator : public UGoalGenerator
{
	GENERATED_BODY()
private:
	UPROPERTY() const TObjectPtr<USplineComponent> Spline;
public:
	virtual bool CanBeGenerated() const override;
	virtual EBasicEnemyGoalType GetType() const override;
	const USplineComponent* GetGoal() const;
};

UCLASS(BlueprintType)
class CYBERPUNKDEMO_API USearchGoalGenerator : public UGoalGenerator
{
	GENERATED_BODY()
	
private:
	const FVector Location;
	
public:
	virtual bool CanBeGenerated() const override;
	virtual EBasicEnemyGoalType GetType() const override;
	const FVector* GetGoal() const;
};

UCLASS(BlueprintType)
class CYBERPUNKDEMO_API UReactionGoalGenerator : public UObject
{
	GENERATED_BODY()

public:
	bool CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge, const FBasicEnemySharedKnowledge& SharedKnowledge);
};

UCLASS(BlueprintType)
class CYBERPUNKDEMO_API UCombatGoalGenerator : public UGoalGenerator
{
	GENERATED_BODY()
private:
	UPROPERTY() const TObjectPtr<APawn> Player;
	
public:
	virtual bool CanBeGenerated() const override;
	virtual EBasicEnemyGoalType GetType() const override;
	const APawn* GetGoal() const;
};

UCLASS(BlueprintType)
class CYBERPUNKDEMO_API UCoverGoalGenerator : public UObject
{
	GENERATED_BODY()

public:
	bool CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge);
};