// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemyController.h"
#include "AI/Actuation/SettableStateTreeComponent.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyConfigData.h"
#include "AI/BasicEnemy/Knowledge/BasicEnemyKnowledgeComponent.h"
#include "AI/BasicEnemy/Knowledge/BasicEnemyPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AI/AIZone/AIZone.h"

ABasicEnemyController::ABasicEnemyController()
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


void ABasicEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!InPawn) return;

	// Initialize AI components
	if (BasicEnemy = Cast<ABasicEnemy>(InPawn))
	{
		if (BasicEnemy->AIZone)
		{
			BasicEnemy->AIZone->OnPlayerIsSensedDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifyPlayerWasSeenAcrossNetwork);
			BasicEnemy->AIZone->OnCombatTimerFinishedDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifyCombatTimerFinished);
			BasicEnemy->AIZone->OnAlertedTimerFinishedDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifyAlertedTimerFinished);
		}


		if (UBasicEnemyConfigData* ConfigData = BasicEnemy->ConfigData)
		{
			// Initialization of Perception Component
			if (UBasicEnemyPerceptionComponent* BasicEnemyPerceptionComponent = Cast<UBasicEnemyPerceptionComponent>(GetPerceptionComponent()))
			{
				if (ConfigData->PerceptionConfigData)
				{
					BasicEnemyPerceptionComponent->SetUpFromData(ConfigData->PerceptionConfigData);
				}
			}

			// Initialization of Knowledge Component From Data
			if (KnowledgeComponent && ConfigData->SensorsConfigData)
			{
				KnowledgeComponent->SetUpFromData(ConfigData->SensorsConfigData);
			}

			// Initialization of Brain
			if (ConfigData->BrainConfigData)
			{
				StateMachine->SetTree(ConfigData->BrainConfigData->Brain.GetMutableStateTree());
			}
		}
	}

	// Initialization of Knowledge Component
	{
		if (KnowledgeComponent)
		{
			KnowledgeComponent->Initialize(Cast<UBasicEnemyPerceptionComponent>(PerceptionComponent), this);
			KnowledgeComponent->OnPlayerSeenDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifyPlayerWasSeen);
			KnowledgeComponent->OnSomethingHeardDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifySomethingWasHeard);
			KnowledgeComponent->OnSoundForgottenDelegate.AddUniqueDynamic(this, &ABasicEnemyController::NotifySoundForgotten);
		}
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

void ABasicEnemyController::NotifyPlayerWasSeen(const APawn* Notifier)
{
	if (StateMachine->IsRunning())
	{
		StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
	}
}

void ABasicEnemyController::NotifyPlayerWasSeenAcrossNetwork(const APawn* Notifier)
{
	if (StateMachine->IsRunning() && Notifier != BasicEnemy)
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