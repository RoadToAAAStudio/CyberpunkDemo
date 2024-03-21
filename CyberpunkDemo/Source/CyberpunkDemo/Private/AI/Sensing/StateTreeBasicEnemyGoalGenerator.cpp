// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Sensing/StateTreeBasicEnemyGoalGenerator.h"
#include "StateTreeExecutionContext.h"
#include "Perception/AIPerceptionTypes.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(StateTreeBasicEnemyGoalGenerator)

void FStateTreeBasicEnemyEvaluator::NotifySomethingWasHeard(const FAIStimulus Stimulus)
{
	
}

void FStateTreeBasicEnemyEvaluator::TreeStart(FStateTreeExecutionContext& Context) const
{

}

void FStateTreeBasicEnemyEvaluator::TreeStop(FStateTreeExecutionContext& Context) const
{

}

void FStateTreeBasicEnemyEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

}
