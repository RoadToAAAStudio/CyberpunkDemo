// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/DecisionMaking/MainComponent/BasicEnemyBrainComponent.h"


UBasicEnemyBrainComponent::UBasicEnemyBrainComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	KnowledgeComponent = nullptr;
}

void UBasicEnemyBrainComponent::TickComponent(float DeltaTime, ELevelTick TickType,	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UBasicEnemyBrainComponent::SetUpFromData(const UDataTable* ConfigData)
{
}

void UBasicEnemyBrainComponent::Initialize(const UBasicEnemyKnowledgeComponent* KnowledgeComponentInput)
{
	KnowledgeComponent = KnowledgeComponentInput;
}
