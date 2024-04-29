// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemyController.h"

#include "StateTree.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyConfigData.h"
#include "AI/Knowledge/MainComponent/BasicEnemyKnowledgeComponent.h"
#include "AI/Knowledge/BasicEnemyPerceptionComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

// ABasicEnemyController::ABasicEnemyController(const FObjectInitializer& ObjectInitializer)
// {
// 	SetupPerceptionSystem();
// }
//
// void ABasicEnemyController::AcceptStateTreeNotification_Implementation(const FName& SourceStateName, const FName& CurrentStateName)
// {
// 	IStateTreeNotificationsAcceptor::AcceptStateTreeNotification_Implementation(SourceStateName, CurrentStateName);
//
// 	const UEnum* GoalEnum = FindFirstObjectSafe<UEnum>(TEXT("EBasicEnemyState"));
// 	if (!GoalEnum) return;
//
// 	int32 Index = GoalEnum->GetIndexByName(SourceStateName);
// 	EBasicEnemyState SourceState = Index != INDEX_NONE? static_cast<EBasicEnemyState>(Index) : EBasicEnemyState::None;
// 	
// 	Index = GoalEnum->GetIndexByName(CurrentStateName);
// 	EBasicEnemyState NewState = Index != INDEX_NONE? static_cast<EBasicEnemyState>(Index) : EBasicEnemyState::None;
//
// 	PersonalKnowledge.CurrentState = NewState;
// 	
// 	StateChanged(SourceState, NewState);
// 	OnBasicEnemyStateChangedDelegate.Broadcast(SourceState, NewState);
// }
//
// #pragma region KNOWELEDGE_GETTERS
// // FVector ABasicEnemyController::GetSpawnLocation() const
// // {
// // 	return PersonalKnowledge.SpawnLocation;
// // }
// //
// // float ABasicEnemyController::GetDistanceFromSpawn() const
// // {
// // 	return PersonalKnowledge.DistanceFromSpawn;
// // }
// //
// // const ASplineContainer* ABasicEnemyController::GetPatrolSpline() const
// // {
// // 	return PersonalKnowledge.PatrolSpline;
// // }
// //
// // const ACharacter* ABasicEnemyController::GetPlayerInSightCone() const
// // {
// // 	return PersonalKnowledge.PlayerInSightCone;
// // }
// //
// // float ABasicEnemyController::GetDistanceFromPlayer() const
// // {
// // 	return PersonalKnowledge.DistanceFromPlayer;
// // }
// //
// // FVector ABasicEnemyController::GetSensedLocation() const
// // {
// // 	return PersonalKnowledge.CurrentHeardStimulus.StimulusLocation;
// // }
// //
// // bool ABasicEnemyController::IsSensedLocationSet() const
// // {
// // 	return PersonalKnowledge.HeartStimulusIsSet();
// // }
// //
// // bool ABasicEnemyController::IsCoverLocationSet() const
// // {
// // 	return PersonalKnowledge.CoverLocationIsSet();
// // }
// //
// // FVector ABasicEnemyController::GetCoverLocation() const
// // {
// // 	return PersonalKnowledge.CurrentCoverLocation;
// // }
// //
// // FGameplayTagContainer ABasicEnemyController::GetTags() const
// // {
// // 	return PersonalKnowledge.Tags;
// // }
// //
// // const TMap<EBasicEnemyGoalType, UGoalGenerator*>& ABasicEnemyController::GetGeneratedGoals() const
// // {
// // 	return PersonalKnowledge.GeneratedGoals;
// // }
// //
// // EBasicEnemyState ABasicEnemyController::GetCurrentState() const
// // {
// // 	return PersonalKnowledge.CurrentState;
// // }
//
// const FBasicEnemyPersonalKnowledge& ABasicEnemyController::GetPersonalKnowledge() const
// {
// 	return PersonalKnowledge;
// }
//
// const FBasicEnemySharedKnowledge& ABasicEnemyController::GetSharedKnowledge() const
// {
// 	return *SharedKnowledge;
// }
//
// #pragma endregion 
//
// #pragma region SENSORS_CONTROL
// void ABasicEnemyController::EnableSightSense(bool Enable)
// {
// 	if (Enable)
// 	{
// 		PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
// 		SightBar->OnBarFilledDelegate.AddDynamic(this, &ABasicEnemyController::NotifySightBarFull);
// 		SightBar->Reset();
//
// 		// TODO What if the player is already in the cone?
//
// 		OnSightSenseToggledDelegate.Broadcast(true);
// 	}
// 	else
// 	{
// 		PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
// 		PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
// 		PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
// 		SightBar->OnBarFilledDelegate.RemoveDynamic(this, &ABasicEnemyController::NotifySightBarFull);
// 		SightBar->Reset();
// 		
// 		OnSightSenseToggledDelegate.Broadcast(false);
// 	}
// }
//
// void ABasicEnemyController::EnableHearingSense(bool Enable)
// {
// 	if (Enable)
// 	{
// 		PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
// 		HearingBar->OnBarFilledDelegate.AddDynamic(this, &ABasicEnemyController::NotifyHearingBarFull);
// 		HearingBar->Reset();
//
// 		OnHearingSenseToggledDelegate.Broadcast(true);
// 	}
// 	else
// 	{
// 		PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
// 		HearingBar->OnBarFilledDelegate.RemoveDynamic(this, &ABasicEnemyController::NotifyHearingBarFull);
// 		HearingBar->Reset();
// 		
// 		OnHearingSenseToggledDelegate.Broadcast(false);
// 	}
// }
//
// bool ABasicEnemyController::IsSightEnabled()
// {
// 	return PersonalKnowledge.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
// }
//
// bool ABasicEnemyController::IsHearingEnabled()
// {
// 	return PersonalKnowledge.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
// }
// #pragma endregion
//
// void ABasicEnemyController::SetupPerceptionSystem()
// {
// 	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
//
// 	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
// 	if (SightConfig)
// 	{
// 		GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
// 		GetPerceptionComponent()->ConfigureSense(*SightConfig);
// 	}
//
// 	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
// 	if (HearingConfig)
// 	{
// 		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
// 	}
//
// 	SightBar = CreateDefaultSubobject<UAttributeBar>(TEXT("SightBar"));
// 	HearingBar = CreateDefaultSubobject<UAttributeBar>(TEXT("HearingBar"));
// }
//
// void ABasicEnemyController::UpdatePersonalKnowledge()
// {
// 	// PlayerDistance
// 	if (SharedKnowledge->Player)
// 	{
// 		PersonalKnowledge.DistanceFromPlayer = FVector::Distance(BasicEnemy->GetActorLocation(), SharedKnowledge->Player->GetActorLocation());
// 	}
// }
//
// void ABasicEnemyController::SensorsUpdate(float DeltaTime)
// {
// 	// TODO Handle locking of sight
// 	
// 	// Sight is Active
// 	if (IsSightEnabled())
// 	{
// 		if (PersonalKnowledge.Tags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone"))))
// 		{
// 			AMainCharacter* Player = Cast<AMainCharacter>(PersonalKnowledge.PlayerInSightCone);
// 			float CrouchMultiplier = Player->GetCustomCharacterComponent()->GetCurrentMovementState() == ECustomMovementState::Crouching? SightCrouchMultiplier : 1.0f;
// 			float DistanceMultiplier = FMath::GetMappedRangeValueClamped(UE::Math::TVector2<float>(0.0f, 2500.0f), UE::Math::TVector2<float>(SightMaxMultiplier, SightMinMultiplier), FVector::Distance(GetPawn()->GetActorLocation(), PersonalKnowledge.PlayerInSightCone->GetActorLocation()));
//
// 			SightBar->AddAmount(SightIncreaseRate * DistanceMultiplier * CrouchMultiplier * DeltaTime);
// 		}
// 		else
// 		{
// 			SightBar->RemoveAmount(SightDecreaseRate * DeltaTime);
// 		}
// 	}
//
// 	// Hearing is Active
// 	if (IsHearingEnabled())
// 	{
// 		if (HearingBar->CurrentValue > 0)
// 		{
// 			HearingBar->RemoveAmount(HearingDecreaseRate * DeltaTime);
// 		}
// 	}
// }
//
// void ABasicEnemyController::GoalGeneration()
// {
// 	TSet<EBasicEnemyGoal>* CurrentGeneratedGoals = &PersonalKnowledge.GeneratedGoals;
// 	
// 	TSet<EBasicEnemyGoal> GoalsToAdd;
// 	TSet<EBasicEnemyGoal> GoalsToRemove;
// 	
// 	// Patrol Goal
// 	if (!CurrentGeneratedGoals->Contains(EBasicEnemyGoal::Patrol))
// 	{
// 		// Try Add
// 		if (PersonalKnowledge.PatrolSpline)
// 		{
// 			GoalsToAdd.Add(EBasicEnemyGoal::Patrol);
// 		}
// 	}
// 	else
// 	{
// 		// Try remove
// 		if (!PersonalKnowledge.PatrolSpline)
// 		{
// 			GoalsToRemove.Remove(EBasicEnemyGoal::Patrol);
// 		}
// 	}
//
// 	// Search Goal
// 	if (!CurrentGeneratedGoals->Contains(EBasicEnemyGoal::Search))
// 	{
// 		// Try Add
// 		if (PersonalKnowledge.bIsHeardStimulusSet || PersonalKnowledge.CurrentState == EBasicEnemyState::Alerted)
// 		{
// 			GoalsToAdd.Add(EBasicEnemyGoal::Search);
// 		}
// 	}
// 	else
// 	{
// 		// Try remove
// 		if (!PersonalKnowledge.bIsHeardStimulusSet && PersonalKnowledge.CurrentState != EBasicEnemyState::Alerted)
// 		{
// 			GoalsToRemove.Remove(EBasicEnemyGoal::Search);
// 		}
// 	}
//
// 	// Combat Goal
// 	if (!CurrentGeneratedGoals->Contains(EBasicEnemyGoal::Combat))
// 	{
// 		// Try Add
// 		if (SharedKnowledge->GetPlayer())
// 		{
// 			GoalsToAdd.Add(EBasicEnemyGoal::Combat);
// 		}
// 	}
// 	else
// 	{
// 		// Try remove
// 		if (!SharedKnowledge->GetPlayer())
// 		{
// 			GoalsToRemove.Remove(EBasicEnemyGoal::Combat);
// 		}
// 	}
//
// 	// Cover Goal
// 	if (!CurrentGeneratedGoals->Contains(EBasicEnemyGoal::Cover))
// 	{
// 		// Try Add
// 		if (PersonalKnowledge.bIsCoverLocationSet)
// 		{
// 			GoalsToAdd.Add(EBasicEnemyGoal::Cover);
// 		}
// 	}
// 	else
// 	{
// 		// Try remove
// 		if (!PersonalKnowledge.bIsCoverLocationSet)
// 		{
// 			GoalsToRemove.Remove(EBasicEnemyGoal::Cover);
// 		}
// 	}
//
// 	CurrentGeneratedGoals->Append(GoalsToAdd);
// 	for (const auto& Goal : GoalsToRemove)
// 	{
// 		CurrentGeneratedGoals->Remove(Goal);
// 	}
//
// 	if (GoalsToAdd.Num() > 0 || GoalsToRemove.Num() > 0)
// 	{		
// 		GoalsChanged(GoalsToAdd, GoalsToRemove);
// 		OnGoalsChanged.Broadcast(GoalsToAdd, GoalsToRemove);
// 	}
// }
//
// #pragma region FUNCTIONS_LISTENERS
// void ABasicEnemyController::NotifyReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus)
// {
// 	switch (Stimulus.Type)
// 	{
// 	case 0:
// 		if (!IsSightEnabled()) break;
//
// 		if (Actor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
// 		{
// 			if (Stimulus.WasSuccessfullySensed())
// 			{
// 				PersonalKnowledge.PlayerInSightCone = Cast<ACharacter>(Actor);
// 				PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
// 				PlayerEnteredInSightCone();
// 				OnPlayerEnteredInSightConeDelegate.Broadcast(this);
// 			}
// 			else
// 			{
// 				PersonalKnowledge.PlayerInSightCone = nullptr;
// 				PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
// 				PersonalKnowledge.Tags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
// 				PlayerExitedFromSightCone();
// 				OnPlayerExitedFromSightConeDelegate.Broadcast(this);
// 			}
// 		}
// 		break;
// 	case 1:
// 		if (!IsHearingEnabled()) break;
//
// 		if (Stimulus.WasSuccessfullySensed())
// 		{
// 			//TODO Amount depends on the Stimulus
// 			CurrentHeardStimulus = Stimulus;
// 			HearingBar->AddAmount(1.0f);
// 		}
// 		else
// 		{
// 			if (PersonalKnowledge.CurrentHeardStimulus.StimulusLocation == Stimulus.StimulusLocation)
// 			{
// 				PersonalKnowledge.UnsetHeardStimulus();
// 			}
// 		}
// 		
// 		break;
// 	default:
// 		break;
// 	}
// }
//
// void ABasicEnemyController::NotifySightBarFull()
// {
// 	PersonalKnowledge.Tags.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
// 	PlayerSeen();
// 	OnPlayerSeenDelegate.Broadcast(this);
// }
//
// void ABasicEnemyController::NotifyHearingBarFull()
// {
// 	PersonalKnowledge.SetHeardStimulus(CurrentHeardStimulus);
// 	SomethingWasHeard(PersonalKnowledge.CurrentHeardStimulus);
// 	OnSomethingWasHeardDelegate.Broadcast(PersonalKnowledge.CurrentHeardStimulus);
// }
// #pragma endregion 
//
// #pragma region FUNCTIONS_OVERRIDES
// void ABasicEnemyController::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// 	SensorsUpdate(DeltaTime);
// 	UpdatePersonalKnowledge();
// 	GoalGeneration();
// }
//
// void ABasicEnemyController::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	EnableSightSense(true);
// 	EnableHearingSense(true);
//
// 	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicEnemyController::NotifyReceiveStimulus);
// }
//
// void ABasicEnemyController::OnPossess(APawn* PossessedPawn)
// {
// 	Super::OnPossess(PossessedPawn);
//
// 	BasicEnemy = Cast<ABasicEnemy>(PossessedPawn);
//
// 	PersonalKnowledge.SpawnLocation = PossessedPawn->GetActorLocation();
// 	PersonalKnowledge.PatrolSpline = Cast<ABasicEnemy>(PossessedPawn)->PatrolSpline;
// }
// #pragma endregion

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
		StateMachine = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateMachine"));
	}
}

void ABasicEnemyController::Initialize(ABasicEnemy* BasicEnemyInput)
{
	// Initialize AI components
	BasicEnemy = Cast<ABasicEnemy>(BasicEnemyInput);
	if (BasicEnemy)
	{
		UDataTable* BasicEnemyConfigData = BasicEnemy->ConfigData;
		if (BasicEnemyConfigData)
		{
			for (auto& RowName : BasicEnemyConfigData->GetRowNames())
			{
				const FBasicEnemyConfigData* ConfigData = BasicEnemyConfigData->FindRow<FBasicEnemyConfigData>(RowName, "");
				if (!ConfigData) continue;

				// Initialization of Perception Component
				if (Cast<UBasicEnemyPerceptionComponent>(GetPerceptionComponent()))
				{
					Cast<UBasicEnemyPerceptionComponent>(GetPerceptionComponent())->SetUpFromData(ConfigData->KnowledgeConfigData);
				}

				// Initialization of Knowledge Component From Data
				if (KnowledgeComponent)
				{
					KnowledgeComponent->SetUpFromData(ConfigData->KnowledgeConfigData);
				}
				
				// TODO SetUp Other Systems
				
				break;
			}
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
	}

	// Initialize State Machine
	{
		if (StateMachine)
		{
			StateMachine->StartLogic();
		}
	}
}

#if 1
float MacroDuration = 5.0f;
#define PRINT_SCREEN(x) GEngine->AddOnScreenDebugMessage(-1, MacroDuration ? MacroDuration : -1.f, FColor::Yellow, x);
#endif

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
	StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
}

void ABasicEnemyController::NotifyCombatTimerFinished()
{
	StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.CombatTimerFinished")));
}

void ABasicEnemyController::NotifyAlertedTimerFinished()
{
	StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.AlertedTimerFinished")));
}
#pragma endregion 