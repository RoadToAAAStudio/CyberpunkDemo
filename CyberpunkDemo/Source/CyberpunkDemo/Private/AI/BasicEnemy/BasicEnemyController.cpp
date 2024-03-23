// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemyController.h"

#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter/MainCharacter.h"
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

FVector ABasicEnemyController::GetSensedLocation() const
{
	return PersonalKnowledge.CurrentHeardStimulus.StimulusLocation;
}

bool ABasicEnemyController::IsSensedLocationSet() const
{
	return PersonalKnowledge.HeartStimulusIsSet();
}

bool ABasicEnemyController::IsCoverLocationSet() const
{
	return PersonalKnowledge.CoverLocationIsSet();
}

FVector ABasicEnemyController::GetCoverLocation() const
{
	return PersonalKnowledge.CurrentCoverLocation;
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
		if (PersonalKnowledge.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone"))))
		{
			AMainCharacter* Player = Cast<AMainCharacter>(PersonalKnowledge.PlayerInSightCone);
			float CrouchMultiplier = Player->GetCustomCharacterComponent()->GetCurrentMovementState() == ECustomMovementState::Crouching? SightCrouchMultiplier : 1.0f;
			float DistanceMultiplier = FMath::GetMappedRangeValueClamped(UE::Math::TVector2<float>(0.0f, 2500.0f), UE::Math::TVector2<float>(SightMaxMultiplier, SightMinMultiplier), FVector::Distance(GetPawn()->GetActorLocation(), PersonalKnowledge.PlayerInSightCone->GetActorLocation()));

			SightBar->AddAmount(SightIncreaseRate * DistanceMultiplier * CrouchMultiplier * DeltaTime);
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
	if (PersonalKnowledge.bIsHeardStimulusSet || Cast<ABasicEnemy>(GetPawn())->GetCurrentState() == EBasicEnemyState::Alerted)
	{
		CurrentGeneratedGoals->Add(EBasicEnemyGoal::Search);
	}
	else
	{
		CurrentGeneratedGoals->Remove(EBasicEnemyGoal::Search);
	}

	// Combat Goal
	if (SharedKnowledge->GetPlayer())
	{
		CurrentGeneratedGoals->Add(EBasicEnemyGoal::Combat);
	}
	else
	{
		CurrentGeneratedGoals->Remove(EBasicEnemyGoal::Combat);
	}

	// Cover Goal
	if (PersonalKnowledge.bIsCoverLocationSet)
	{
		CurrentGeneratedGoals->Add(EBasicEnemyGoal::Cover);
	}
	else
	{
		CurrentGeneratedGoals->Remove(EBasicEnemyGoal::Cover);
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
				PersonalKnowledge.PlayerInSightCone = Cast<ACharacter>(Actor);
				PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
				PlayerEnteredInSightCone();
				OnPlayerEnteredInSightConeDelegate.Broadcast(this);
			}
			else
			{
				PersonalKnowledge.PlayerInSightCone = nullptr;
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
			CurrentHeardStimulus = Stimulus;
			HearingBar->AddAmount(1.0f);
		}
		else
		{
			if (PersonalKnowledge.CurrentHeardStimulus.StimulusLocation == Stimulus.StimulusLocation)
			{
				PersonalKnowledge.UnsetHeardStimulus();
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
	PersonalKnowledge.SetHeardStimulus(CurrentHeardStimulus);
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