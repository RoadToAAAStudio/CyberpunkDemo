// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemyController.h"

#include "StateTree.h"
#include "AI/Actuation/SettableStateTreeComponent.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyConfigData.h"
#include "AI/Knowledge/MainComponent/BasicEnemyKnowledgeComponent.h"
#include "AI/Knowledge/BasicEnemyPerceptionComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ABasicEnemyController::ABasicEnemyController(const FObjectInitializer& ObjectInitializer)
{
	// Construct Perception Component
	{
		PerceptionComponent = CreateDefaultSubobject<UBasicEnemyPerceptionComponent>(TEXT("PerceptionComponent"));
		SetPerceptionComponent(*PerceptionComponent);

		UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
		if (SightConfig)
		{
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			AAIController::GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
			AAIController::GetPerceptionComponent()->ConfigureSense(*SightConfig);
		}

		UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
		if (HearingConfig)
		{
			SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
			SightConfig->DetectionByAffiliation.bDetectEnemies = true;
			AAIController::GetPerceptionComponent()->ConfigureSense(*HearingConfig);
		}
	}

	// Construct Knowledge Component
	{
		KnowledgeComponent = CreateDefaultSubobject<UBasicEnemyKnowledgeComponent>(TEXT("KnowledgeComponent"));
	}
	
	// Construct State Machine Component
	{
		StateMachine = CreateDefaultSubobject<USettableStateTreeComponent>(TEXT("StateMachine"));
		StateMachine->SetStartLogicAutomatically(false);
	}
}

void ABasicEnemyController::Initialize(ABasicEnemy* BasicEnemyInput)
{
	// Initialize AI components
	BasicEnemy = Cast<ABasicEnemy>(BasicEnemyInput);
	if (BasicEnemy)
	{
		//UDataTable* BasicEnemyConfigData = BasicEnemy->ConfigData;
		UBasicEnemyConfigData* ConfigData = BasicEnemy->ConfigData;
		if (ConfigData)
		{
			// Initialization of Perception Component
			if (Cast<UBasicEnemyPerceptionComponent>(GetPerceptionComponent()))
			{
				Cast<UBasicEnemyPerceptionComponent>(GetPerceptionComponent())->SetUpFromData(ConfigData->PerceptionConfigData);
			}
			// Initialization of Knowledge Component From Data
			if (KnowledgeComponent)
			{
				KnowledgeComponent->SetUpFromData(ConfigData->SensorsConfigData);
			}

			StateMachine->SetTree(ConfigData->Behaviour);
		}
	}

	// Hook to AIZone
	{

		BasicEnemy->AIZone->OnPlayerIsSensedDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifyPlayerWasSeen);
		BasicEnemy->AIZone->OnCombatTimerFinishedDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifyCombatTimerFinished);
		BasicEnemy->AIZone->OnAlertedTimerFinishedDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifyAlertedTimerFinished);
	}
	
	// Initialization of Knowledge Component
	{
		KnowledgeComponent->Initialize(Cast<UBasicEnemyPerceptionComponent>(PerceptionComponent), this, BasicEnemy->AIZone);
		KnowledgeComponent->OnPlayerSeenDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifyPlayerWasSeen);
		KnowledgeComponent->OnSomethingHeardDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifySomethingWasHeard);
		KnowledgeComponent->OnSoundForgottenDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifySoundForgotten);
	}

	// Initialize State Machine
	{
		if (StateMachine)
		{
			StateMachine->StartLogic();
		}
	}
}

void ABasicEnemyController::GetChangeOfState_Implementation(const FName& SourceStateName, const FName& NextStateName)
{
	const UEnum* GoalEnum = FindFirstObjectSafe<UEnum>(TEXT("EBasicEnemyState"));
	if (!GoalEnum) return;

	int32 Index = GoalEnum->GetIndexByName(SourceStateName);
	EBasicEnemyState SourceState = Index != INDEX_NONE? static_cast<EBasicEnemyState>(Index) : EBasicEnemyState::None;

	Index = GoalEnum->GetIndexByName(NextStateName);
	EBasicEnemyState NextState = Index != INDEX_NONE? static_cast<EBasicEnemyState>(Index) : EBasicEnemyState::None;
	
	StateChanged(SourceState, NextState);
	OnBasicEnemyStateChangedDelegate.Broadcast(SourceState, NextState);
}

#pragma region EVENT_LISTENERS
void ABasicEnemyController::NotifyPlayerWasSeen(const APawn* Notifier)
{
	/*
	 * This listener is called when this actor sees the player or someone else did (via AIZone shared knowledge)
	 * This means that it will be called twice on the actor who actually saw the Player
	 * So the message to the StateTree should be called only once
	 */
	if (StateMachine->IsRunning())
	{
		StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
	}
}

void ABasicEnemyController::NotifyCombatTimerFinished()
{
	if (StateMachine->IsRunning())
	{
		
		StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.CombatTimerFinished")));
	}
}

void ABasicEnemyController::NotifyAlertedTimerFinished()
{
	if (StateMachine->IsRunning())
	{
		StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.AlertedTimerFinished")));
	}
}

void ABasicEnemyController::NotifySomethingWasHeard(const APawn* Notifier, const FAIStimulus Stimulus)
{
	if (StateMachine->IsRunning())
	{
		StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.SomethingWasHeard")));
	}
}

void ABasicEnemyController::NotifySoundForgotten(const APawn* Notifier, const FAIStimulus Stimulus)
{
	if (StateMachine->IsRunning())
	{
		StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.SoundForgotten")));
	}
}
#pragma endregion 