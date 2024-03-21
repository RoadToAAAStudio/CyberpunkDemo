// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemyController.h"

#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ABasicEnemyController::ABasicEnemyController(const FObjectInitializer& ObjectInitializer)
{
	SetupPerceptionSystem();
}

#pragma region KNOWELEDGE_GETTERS
FVector ABasicEnemyController::GetSpawnLocation() const
{
	return PersonalKnowledge.SpawnLocation;
}

const ASplineContainer* ABasicEnemyController::GetPatrolSpline() const
{
	return PersonalKnowledge.PatrolSpline;
}

FGameplayTagContainer ABasicEnemyController::GetTags() const
{
	return PersonalKnowledge.Tags;
}

TSet<EBasicEnemyGoal> ABasicEnemyController::GetCurrentGeneratedGoals() const
{
	return PersonalKnowledge.CurrentGeneratedGoals;
}
#pragma endregion 

#pragma region SENSORS_CONTROL
void ABasicEnemyController::EnableSightSense(bool Enable)
{
	if (Enable)
	{
		PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
		SightBar->OnBarFilledDelegate.AddDynamic(this, &ABasicEnemyController::NotifySightBarFull);
		SightBar->Reset();

		// TODO What if the player is already in the cone?

		OnSightSenseToggledDelegate.Broadcast(true);
	}
	else
	{
		PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
		PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
		PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
		SightBar->OnBarFilledDelegate.RemoveDynamic(this, &ABasicEnemyController::NotifySightBarFull);
		SightBar->Reset();
		
		OnSightSenseToggledDelegate.Broadcast(false);
	}
}

void ABasicEnemyController::EnableHearingSense(bool Enable)
{
	if (Enable)
	{
		PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
		HearingBar->OnBarFilledDelegate.AddDynamic(this, &ABasicEnemyController::NotifyHearingBarFull);
		HearingBar->Reset();

		OnHearingSenseToggledDelegate.Broadcast(true);
	}
	else
	{
		PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
		HearingBar->OnBarFilledDelegate.RemoveDynamic(this, &ABasicEnemyController::NotifyHearingBarFull);
		HearingBar->Reset();
		
		OnHearingSenseToggledDelegate.Broadcast(false);
	}
}

bool ABasicEnemyController::IsSightEnabled()
{
	return PersonalKnowledge.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
}

bool ABasicEnemyController::IsHearingEnabled()
{
	return PersonalKnowledge.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
}
#pragma endregion

void ABasicEnemyController::SetupPerceptionSystem()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	if (HearingConfig)
	{
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}

	SightBar = CreateDefaultSubobject<UAttributeBar>(TEXT("SightBar"));
	HearingBar = CreateDefaultSubobject<UAttributeBar>(TEXT("HearingBar"));
}

void ABasicEnemyController::SensorsUpdate(float DeltaTime)
{
	// TODO Handle locking of sight
	
	// Sight is Active
	if (IsSightEnabled())
	{
		// TODO Depends on the crouching and distance
		if (PersonalKnowledge.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone"))))
		{
			SightBar->AddAmount(SightIncreaseRate * DeltaTime);
		}
		else
		{
			SightBar->RemoveAmount(SightDecreaseRate * DeltaTime);
		}
	}

	// Hearing is Active
	if (IsHearingEnabled())
	{
		if (HearingBar->CurrentValue > 0)
		{
			HearingBar->RemoveAmount(HearingDecreaseRate * DeltaTime);
		}
	}
}

void ABasicEnemyController::GoalGeneration()
{
	TSet<EBasicEnemyGoal>* CurrentGeneratedGoals = &PersonalKnowledge.CurrentGeneratedGoals; 
	// Patrol Goal
	if (PersonalKnowledge.PatrolSpline)
	{
		CurrentGeneratedGoals->Add(EBasicEnemyGoal::Patrol);
	}
	else 
	{
		CurrentGeneratedGoals->Remove(EBasicEnemyGoal::Patrol);
	}

	// Search Goal
	//if (PersonalKnowledge.CurrentHeardStimulus)

	// Combat Goal
	if (SharedKnowledge->GetPlayer())
	{
		CurrentGeneratedGoals->Add(EBasicEnemyGoal::Combat);
	}
	else
	{
		CurrentGeneratedGoals->Remove(EBasicEnemyGoal::Combat);
	}
}

#pragma region FUNCTIONS_LISTENERS
void ABasicEnemyController::NotifyReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0:
		if (!IsSightEnabled()) break;

		if (Actor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
				PlayerEnteredInSightCone();
				OnPlayerEnteredInSightConeDelegate.Broadcast(this);
			}
			else
			{
				PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
				PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
				PlayerExitedFromSightCone();
				OnPlayerExitedFromSightConeDelegate.Broadcast(this);
			}
		}
		break;
	case 1:
		if (!IsHearingEnabled()) break;

		if (Stimulus.WasSuccessfullySensed())
		{
			//TODO Amount depends on the Stimulus
			PersonalKnowledge.CurrentHeardStimulus = Stimulus;
			HearingBar->AddAmount(1.0f);

		}
		else
		{
			//TODO Stimulus is forgotten
			if (PersonalKnowledge.CurrentHeardStimulus.StimulusLocation == Stimulus.StimulusLocation)
			{
				PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing.SomethingWasHeard")));
			}
		}
		
		break;
	default:
		break;
	}
}

void ABasicEnemyController::NotifySightBarFull()
{
	PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
	PlayerSeen();
	OnPlayerSeenDelegate.Broadcast(this);
}

void ABasicEnemyController::NotifyHearingBarFull()
{
	PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing.SomethingWasHeard")));
	SomethingWasHeard(PersonalKnowledge.CurrentHeardStimulus);
	OnSomethingWasHeardDelegate.Broadcast(PersonalKnowledge.CurrentHeardStimulus);
}
#pragma endregion 

#pragma region FUNCTIONS_OVERRIDES
void ABasicEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SensorsUpdate(DeltaTime);
	GoalGeneration();
}

void ABasicEnemyController::BeginPlay()
{
	Super::BeginPlay();

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicEnemyController::NotifyReceiveStimulus);
	EnableSightSense(true);
	EnableHearingSense(true);
}

void ABasicEnemyController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);

	if (Cast<ABasicEnemy>(PossessedPawn))
	{
		PersonalKnowledge.PatrolSpline = Cast<ABasicEnemy>(PossessedPawn)->PatrolSpline;
	}
}
#pragma endregion 