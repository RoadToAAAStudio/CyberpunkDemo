// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Knowledge/MainComponent/BasicEnemyKnowledgeComponent.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyConfigData.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "AI/Knowledge/AttributeBar.h"
#include "AI/Knowledge/BasicEnemyPerceptionComponent.h"
#include "AI/Knowledge/GoalGenerators.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter/MainCharacter.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"

// Sets default values for this component's properties
UBasicEnemyKnowledgeComponent::UBasicEnemyKnowledgeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	PersonalKnowledge = FBasicEnemyPersonalKnowledge();
	AIZone = nullptr;
}

// Called every frame
void UBasicEnemyKnowledgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Update Sensors
	{
		if (IsSightEnabled())
		{
			if (PersonalKnowledge.PlayerInSightCone.IsSet())
			{
				AMainCharacter* Player = Cast<AMainCharacter>(PersonalKnowledge.PlayerInSightCone.Get());
				UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSense(UAISenseConfig_Sight::StaticClass()));
	
				// Figure out Crouch Multiplier
				float CrouchMultiplier = 1.0f;
				if (Player->GetCustomCharacterMovementComponent()->GetCurrentMovementState() == ECustomMovementState::Crouching)
				{
					CrouchMultiplier = SightCrouchMultiplier;
				}
	
				// Figure out Distance Multiplier
				float DistanceMultiplier = 1.0f;
				float DistanceFromPlayer = PersonalKnowledge.DistanceFromPlayer.Get();
				UE::Math::TVector2<float> DistanceRange = UE::Math::TVector2(0.0f, SightConfig->SightRadius);
				UE::Math::TVector2<float> MultiplierRange = UE::Math::TVector2(SightDistanceMaxMultiplier, SightDistanceMinMultiplier);
				DistanceMultiplier = FMath::GetMappedRangeValueClamped(DistanceRange, MultiplierRange, DistanceFromPlayer);
			
				SightBar->Add(SightBaseIncreaseRate * DistanceMultiplier * CrouchMultiplier * DeltaTime);
			}
			else
			{
				SightBar->Remove(SightBaseDecreaseRate * DeltaTime);
			}
		}
	
		if (IsHearingEnabled())
		{
			HearingBar->Remove(HearingBaseDecreaseRate * DeltaTime);
		}
	}
	
	// Update Knowledge
	{
		APawn* Agent = PersonalKnowledge.Agent.Get();
		if (Agent)
		{
			PersonalKnowledge.AgentLocation = Agent->GetActorLocation();
			PersonalKnowledge.AgentDistanceFromSpawn = FVector::Distance(PersonalKnowledge.AgentSpawnLocation.Get(), PersonalKnowledge.Agent.Get()->GetActorLocation());
		}
	
		AMainCharacter* Player = (PersonalKnowledge.PlayerInSightCone.Get());
		if (Player)
		{
			PersonalKnowledge.DistanceFromPlayer = FVector::Distance(PersonalKnowledge.AgentLocation.Get(), Player->GetActorLocation());
		}
	}
	
	// Update Goals
	{
		for (auto& GoalGenerator : PersonalKnowledge.GoalGenerators)
		{
			if (GoalGenerator->CanBeGenerated())
			{
				GoalGenerator->Generate();
			}
			else
			{
				GoalGenerator->Destroy();
			}
		}
	}
}

void UBasicEnemyKnowledgeComponent::Initialize(	UBasicEnemyPerceptionComponent* PerceptionComponentInput,
												ABasicEnemyController* BasicEnemyControllerInput,
												AAIZone* AIZoneInput)
{
	SightBar = NewObject<UAttributeBar>();
	HearingBar = NewObject<UAttributeBar>();

	
	PerceptionComponent = PerceptionComponentInput;
	BasicEnemyController = BasicEnemyControllerInput;
	AIZone = AIZoneInput;

	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &UBasicEnemyKnowledgeComponent::NotifyReceiveStimulus);
	BasicEnemyController->OnBasicEnemyStateChangedDelegate.AddUniqueDynamic(this, &UBasicEnemyKnowledgeComponent::NotifyStateChanged);

	// Initialize Sensors
	{
		ToggleSight(true);
		ToggleHearing(true);
	}

	// Initialize Knowledge
	{
		PersonalKnowledge.Agent = Cast<AController>(GetOwner())->GetPawn();
		PersonalKnowledge.AgentSpawnLocation = PersonalKnowledge.Agent.Get()->GetActorLocation();

		PersonalKnowledge.AgentState = EBasicEnemyState::Unaware;

		ASplineContainer* SplineContainer = Cast<ABasicEnemy>(Cast<AController>(GetOwner())->GetPawn())->PatrolSpline;
		PersonalKnowledge.PatrolSpline = SplineContainer? Cast<USplineComponent>(SplineContainer->GetComponentByClass(USplineComponent::StaticClass())) : nullptr;
	}

	// Initialize Generators
	{
		
	}
}

void UBasicEnemyKnowledgeComponent::SetUpFromData(const UDataTable* ConfigDataTable)
{
	const FKnowledgeConfigData* ConfigData = nullptr;
	for (auto& RowName : ConfigDataTable->GetRowNames())
	{
		ConfigData = ConfigDataTable->FindRow<FKnowledgeConfigData>(RowName, "");
		if (!ConfigData) continue;
		break;
	}
	if (!ConfigData) return;
	
	// Set Up Sensors
	{
		SightBaseIncreaseRate		= ConfigData->SightBaseIncreaseRate;			
		SightBaseDecreaseRate		= ConfigData->SightBaseDecreaseRate;			
		SightCrouchMultiplier		= ConfigData->SightCrouchMultiplier;			
		SightDistanceMinMultiplier	= ConfigData->SightDistanceMinMultiplier;	
		SightDistanceMaxMultiplier	= ConfigData->SightDistanceMaxMultiplier;

		HearingBaseDecreaseRate		= ConfigData->HearingBaseDecreaseRate;
	}

	// Set Up Knowledge
	{

	}
	
	// Set Up Goal Generators
	{
		for (const TSubclassOf<UGoalGenerator>& GoalGeneratorClass : ConfigData->SupportedGoals)
		{
			UGoalGenerator* GoalGenerator = NewObject<UGoalGenerator>(this, GoalGeneratorClass);
			GoalGenerator->Initialize(this);
			PersonalKnowledge.GoalGenerators.Add(GoalGenerator);
		}
	}
}

const FBasicEnemyPersonalKnowledge& UBasicEnemyKnowledgeComponent::GetPersonalKnowledge() const
{
	return PersonalKnowledge;
}
const AAIZone* UBasicEnemyKnowledgeComponent::GetAIZone() const
{
	return AIZone;
}

#pragma region SENSORS_PUBLIC_CONTROLS
bool UBasicEnemyKnowledgeComponent::IsSightEnabled		()  const { return bSightEnabled;			}
bool UBasicEnemyKnowledgeComponent::IsHearingEnabled	()  const { return bHearingEnabled;			}
float UBasicEnemyKnowledgeComponent::GetSightBarValue	()  const { return SightBar->GetValue();	}
float UBasicEnemyKnowledgeComponent::GetHearingValue	()  const { return HearingBar->GetValue();	}

void UBasicEnemyKnowledgeComponent::ToggleSight(bool Enable)
{
	if (Enable)
	{
		SightBar->OnBarFilledDelegate.AddUniqueDynamic(this, &UBasicEnemyKnowledgeComponent::NotifySightBarFull);
		SightBar->OnBarEmptiedDelegate.AddUniqueDynamic(this, &UBasicEnemyKnowledgeComponent::NotifySightBarEmpty);
	}
	else
	{
		SightBar->OnBarFilledDelegate.RemoveDynamic(this, &UBasicEnemyKnowledgeComponent::NotifySightBarFull);
		SightBar->OnBarEmptiedDelegate.RemoveDynamic(this, &UBasicEnemyKnowledgeComponent::NotifySightBarEmpty);
	}
	
	bSightEnabled = Enable;
	SightBar->Reset();
	SenseToggled(UAISense_Sight::StaticClass(), Enable);
	OnSenseToggledDelegate.Broadcast(UAISense_Sight::StaticClass(), Enable);
}

void UBasicEnemyKnowledgeComponent::ToggleHearing(bool Enable)
{
	if (Enable)
	{
		HearingBar->OnBarFilledDelegate.AddUniqueDynamic(this, &UBasicEnemyKnowledgeComponent::NotifyHearingBarFull);
	}
	else
	{
		HearingBar->OnBarFilledDelegate.RemoveDynamic(this, &UBasicEnemyKnowledgeComponent::NotifyHearingBarFull);
	}
	
	bHearingEnabled = Enable;
	HearingBar->Reset();
	SenseToggled(UAISense_Hearing::StaticClass(), Enable);
	OnSenseToggledDelegate.Broadcast(UAISense_Hearing::StaticClass(), Enable);
}
#pragma endregion

#pragma region EVENT_LISTENERS
void UBasicEnemyKnowledgeComponent::NotifyReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus)
{
	if (IsSightEnabled() && Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSenseConfig(Stimulus.Type)))
	{
		// Was the player ?
		if (Actor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0) && Cast<AMainCharacter>(Actor))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				PersonalKnowledge.PlayerInSightCone.Set(Cast<AMainCharacter>(Actor));
				PlayerEnteredSightCone();
				OnPlayerEnteredSightConeDelegate.Broadcast(PersonalKnowledge.Agent.Get());
			}
			else
			{
				PersonalKnowledge.PlayerInSightCone.UnSet();
				PlayerExitedSightCone();
				OnPlayerExitedSightConeDelegate.Broadcast(PersonalKnowledge.Agent.Get());
			}
		}
	}
	else if (IsHearingEnabled() && Cast<UAISenseConfig_Hearing>(PerceptionComponent->GetSenseConfig(Stimulus.Type)))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			//TODO Amount depends on the Stimulus
			TemporaryHeardStimulus = Stimulus;
			HearingBar->Add(1.0f);
		}
		else
		{
			if (Stimulus.StimulusLocation == PersonalKnowledge.HeardStimulus.Get().StimulusLocation)
			{
				PersonalKnowledge.HeardStimulus.UnSet();
			}
			SoundForgotten(Stimulus);
			OnSoundForgottenDelegate.Broadcast(PersonalKnowledge.Agent.Get(), Stimulus);
		}
	}
}

void UBasicEnemyKnowledgeComponent::NotifyStateChanged(EBasicEnemyState OldState, EBasicEnemyState NewState)
{
	PersonalKnowledge.AgentState = NewState;
}

void UBasicEnemyKnowledgeComponent::NotifySightBarFull()
{
	PlayerSeen();
	OnPlayerSeenDelegate.Broadcast(PersonalKnowledge.Agent.Get());
}

void UBasicEnemyKnowledgeComponent::NotifySightBarEmpty()
{
	PlayerHidden();
	OnPlayerHiddenDelegate.Broadcast(PersonalKnowledge.Agent.Get());
}

void UBasicEnemyKnowledgeComponent::NotifyHearingBarFull()
{
	PersonalKnowledge.HeardStimulus.Set(TemporaryHeardStimulus);
	SomethingHeard(TemporaryHeardStimulus);
	OnSomethingHeardDelegate.Broadcast(PersonalKnowledge.Agent.Get(), TemporaryHeardStimulus);
}
#pragma endregion