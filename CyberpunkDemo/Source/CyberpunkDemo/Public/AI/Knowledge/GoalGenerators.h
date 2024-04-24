// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GoalGenerators.generated.h"

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

/**
 *  Possible goals:
 *  Patrol,
 *  Search,
 *  Reaction,
 *  Combat,
 *  Cover,
 */
//
// UCLASS()
// class CYBERPUNKDEMO_API UGoalGenerator : public UObject
// {
// 	GENERATED_BODY()
//
// private:
// 	bool Destroyed = true;
// 	EBasicEnemyGoalType Type = EBasicEnemyGoalType::None;
// 	
// public:
// 	bool IsDestroyed() const { return Destroyed; }
// 	EBasicEnemyGoalType GetType() const { return Type; }
// 	virtual bool CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge, const FBasicEnemySharedKnowledge& SharedKnowledge);
// };
//
// UCLASS()
// class CYBERPUNKDEMO_API UPatrolGoalGenerator : public UGoalGenerator
// {
// 	GENERATED_BODY()
//
// public:
// 	virtual bool CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge, const FBasicEnemySharedKnowledge& SharedKnowledge) override;
// };
//
// UCLASS()
// class CYBERPUNKDEMO_API USearchGoalGenerator : public UGoalGenerator
// {
// 	GENERATED_BODY()
//
// public:
// 	virtual bool CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge, const FBasicEnemySharedKnowledge& SharedKnowledge) override;
// };
//
// UCLASS()
// class CYBERPUNKDEMO_API UReactionGoalGenerator : public UGoalGenerator
// {
// 	GENERATED_BODY()
//
// public:
// 	virtual bool CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge, const FBasicEnemySharedKnowledge& SharedKnowledge) override;
// };
//
// UCLASS()
// class CYBERPUNKDEMO_API UCombatGoalGenerator : public UGoalGenerator
// {
// 	GENERATED_BODY()
//
// public:
// 	virtual bool CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge, const FBasicEnemySharedKnowledge& SharedKnowledge) override;
// };
//
// UCLASS()
// class CYBERPUNKDEMO_API UCoverGoalGenerator : public UGoalGenerator
// {
// 	GENERATED_BODY()
//
// public:
// 	virtual bool CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge, const FBasicEnemySharedKnowledge& SharedKnowledge) override;
// };