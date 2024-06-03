// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "AI/Knowledge/MainComponent/BasicEnemyKnowledgeComponent.h"
#include "AI/WorldInterfacing/Location.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAIZone::AAIZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateMachine = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>("BoxTrigger");
	// bGenerateOverlapEventsDuringLevelStreaming = true;
	// BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAIZone::NotifySomethingEnteredInTheTrigger);
}

void AAIZone::RegisterBasicEnemy(ABasicEnemy* NewBasicEnemy)
{
	if (NewBasicEnemy == nullptr) return;

	SharedKnowledge.Enemies.AddUnique(NewBasicEnemy);

	ABasicEnemyController* EnemyController = Cast<ABasicEnemyController>(NewBasicEnemy->GetController());
	if (EnemyController)
	{
		EnemyController->KnowledgeComponent->OnPlayerEnteredSightConeDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerEnteredInSightCone);
		EnemyController->KnowledgeComponent->OnPlayerExitedSightConeDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerExitedInSightCone);
		EnemyController->KnowledgeComponent->OnPlayerSeenDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerWasSeen);
	}
}

void AAIZone::RegisterCover(ALocation* NewCover)
{
	if (NewCover == nullptr) return;

	ALocation* Location = Cast<ALocation>(NewCover);
	SharedKnowledge.CoverPerLocations.Add(Location->GetActorLocation(), Location);
}

void AAIZone::GetChangeOfState_Implementation(const FName& SourceStateName, const FName& CurrentStateName)
{
	const UEnum* GoalEnum = FindFirstObjectSafe<UEnum>(TEXT("EAIZoneState"));
	if (!GoalEnum) return;

	int32 Index = GoalEnum->GetIndexByName(SourceStateName);
	EAIZoneState SourceState = Index != INDEX_NONE? static_cast<EAIZoneState>(Index) : EAIZoneState::None;
	
	Index = GoalEnum->GetIndexByName(CurrentStateName);
	EAIZoneState NewState = Index != INDEX_NONE? static_cast<EAIZoneState>(Index) : EAIZoneState::None;
	
	SharedKnowledge.AIZoneState = NewState;
	
	switch (SourceState)
	{
		case EAIZoneState::Unaware:
			break;
			
		case EAIZoneState::Combat:
		{
			SharedKnowledge.Player = nullptr;
			PlayerForgotten();
			OnPlayerIsForgottenDelegate.Broadcast();
		}
		break;
			
		case EAIZoneState::Alerted:
			break;
			
		case EAIZoneState::Max:
			break;
	}
	
	switch (NewState)
	{
		case EAIZoneState::Unaware:
			break;
			
		case EAIZoneState::Combat:
			break;
			
		case EAIZoneState::Alerted:
		{
			// Start Alerted Timer
			FTimerDelegate TimerCallback = FTimerDelegate::CreateLambda([this]()
			{
				StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.AlertedTimerFinished")));
				SharedKnowledge.AlertedTimer.UnSet();
				AlertedTimerFinished();
				OnAlertedTimerFinishedDelegate.Broadcast();
			});

			GetWorld()->GetTimerManager().SetTimer(SharedKnowledge.AlertedTimer.Get(), TimerCallback, SharedKnowledge.AlertedTimerDuration.Get(), false);
			SharedKnowledge.AlertedTimer.Set();
			AlertedTimerStarted();
			OnAlertedTimerStartedDelegate.Broadcast();
		}
		break;
			
		case EAIZoneState::Max:
			break;
	}
	
	StateChanged(SourceState, NewState);
	OnAIZoneManagerStateChangedDelegate.Broadcast(SourceState, NewState);
}

void AAIZone::BeginPlay()
{
	Super::BeginPlay();

	SharedKnowledge.CombatTimerDuration = CombatTimerDuration;
	SharedKnowledge.AlertedTimerDuration = AlertedTimerDuration;
}

#pragma region FUNCTIONS_LISTENERS
void AAIZone::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (Cast<ALocation>(OtherActor))
	// {
	// 	ALocation* Location = Cast<ALocation>(OtherActor);
	// 	SharedKnowledge.CoverPerLocations.Add(Location->GetActorLocation(), Location);
	// }
	// else if (Cast<ABasicEnemy>(OtherActor))
	// {
	// 	ABasicEnemy* Enemy = Cast<ABasicEnemy>(OtherActor);
	// 	SharedKnowledge.Enemies.Add(Enemy);
	// 	
	// 	ABasicEnemyController* EnemyController = Cast<ABasicEnemyController>(Enemy->GetController());
	// 	if (EnemyController)
	// 	{
	// 		EnemyController->KnowledgeComponent->OnPlayerEnteredSightConeDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerEnteredInSightCone);
	// 		EnemyController->KnowledgeComponent->OnPlayerExitedSightConeDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerExitedInSightCone);
	// 		EnemyController->KnowledgeComponent->OnPlayerSeenDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerWasSeen);
	// 	}
	// }
}

void AAIZone::NotifyPlayerEnteredInSightCone(const APawn* PawnOwner)
{
	SharedKnowledge.NumberOfSightConesThePlayerIsIn = SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get() + 1;

	if (SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get() == 1)
	{
		if (SharedKnowledge.AIZoneState == EAIZoneState::Combat)
		{
			GetWorld()->GetTimerManager().ClearTimer(SharedKnowledge.CombatTimer.Get());
			SharedKnowledge.CombatTimer.UnSet();
		}
	}
}

void AAIZone::NotifyPlayerExitedInSightCone(const APawn* PawnOwner)
{
	SharedKnowledge.NumberOfSightConesThePlayerIsIn = SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get() - 1;

	if (SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get() > 0) return;

	if (SharedKnowledge.AIZoneState == EAIZoneState::Combat)
	{
		FTimerDelegate TimerCallback = FTimerDelegate::CreateLambda([this]()
		{
			StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.CombatTimerFinished")));
			SharedKnowledge.CombatTimer.UnSet();
			CombatTimerFinished();
			OnCombatTimerFinishedDelegate.Broadcast();
		});

		GetWorld()->GetTimerManager().SetTimer(SharedKnowledge.CombatTimer.Get(), TimerCallback, SharedKnowledge.CombatTimerDuration.Get(), false);
		SharedKnowledge.CombatTimer.Set();
		CombatTimerStarted();
		OnCombatTimerStartedDelegate.Broadcast();
	}
	PlayerInNoSightCone();
	OnPlayerIsInNoSightConeDelegate.Broadcast();
}

void AAIZone::NotifyPlayerWasSeen(const APawn* PawnOwner)
{
	if (SharedKnowledge.AIZoneState == EAIZoneState::Unaware || SharedKnowledge.AIZoneState == EAIZoneState::Alerted)
	{
		if (SharedKnowledge.AIZoneState == EAIZoneState::Alerted)
		{
			GetWorld()->GetTimerManager().ClearTimer(SharedKnowledge.AlertedTimer.Get());
			SharedKnowledge.AlertedTimer.UnSet();
		}
		SharedKnowledge.Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		PlayerSensed();
		OnPlayerIsSensedDelegate.Broadcast(PawnOwner);
		StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
	}
}
#pragma endregion 