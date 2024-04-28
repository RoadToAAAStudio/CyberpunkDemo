// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Knowledge/GoalGenerators.h"
#include "AI/Knowledge/MainComponent/BasicEnemyKnowledgeComponent.h"

#pragma region PATROL
bool UPatrolGoalGenerator::CanBeGenerated() const
{
	return Knowledge->GetPersonalKnowledge().PatrolSpline.IsSet();
}

EBasicEnemyGoalType UPatrolGoalGenerator::GetType() const
{
	return EBasicEnemyGoalType::Patrol;
}

const USplineComponent* UPatrolGoalGenerator::GetGoal() const
{
	if (!bIsGenerated) return nullptr;
	
	return Spline;
}
#pragma endregion

#pragma region SEARCH
bool USearchGoalGenerator::CanBeGenerated() const
{
	return Knowledge->GetPersonalKnowledge().HeardStimulus.IsSet() || Knowledge->GetPersonalKnowledge().AgentState == EBasicEnemyState::Alerted;
}

EBasicEnemyGoalType USearchGoalGenerator::GetType() const
{
	return EBasicEnemyGoalType::Search;
}

const FVector* USearchGoalGenerator::GetGoal() const
{
	if (!bIsGenerated) return nullptr;
	
	return &Location;
}
#pragma endregion 

bool UReactionGoalGenerator::CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge, const FBasicEnemySharedKnowledge& SharedKnowledge)
{
	return false;
}

#pragma region COMBAT
bool UCombatGoalGenerator::CanBeGenerated() const
{
	if (!Knowledge->GetAIZone()) return false;
	return Knowledge->GetAIZone()->GetSharedKnowledge().Player.IsSet();
}

EBasicEnemyGoalType UCombatGoalGenerator::GetType() const
{
	return EBasicEnemyGoalType::Combat;
}

const APawn* UCombatGoalGenerator::GetGoal() const
{
	return Knowledge->GetAIZone()->GetSharedKnowledge().Player.Get();
}
#pragma endregion 

bool UCoverGoalGenerator::CanBeGenerated(const FBasicEnemyPersonalKnowledge& PersonalKnowledge)
{
	return PersonalKnowledge.CoverLocation.IsSet();
}
