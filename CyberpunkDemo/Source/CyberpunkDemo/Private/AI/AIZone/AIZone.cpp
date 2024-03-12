// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIZone/AIZone.h"
#include "AI/Utility/AIUtility.h"

// Sets default values
AAIZone::AAIZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}

const AActor* AAIZone::GetPlayer()
{
	return Player;
}

FTimerHandle AAIZone::GetCombatTimerHandle()
{
	return CombatTimer;
}

FTimerHandle AAIZone::GetAlertedimerHandle()
{
	return AlertedTimer;
}

int AAIZone::GetNumberOfSightConesThePlayerIsIn()
{
	return NumberOfSightConesThePlayerIsIn;
}

const TArray<ABasicEnemy*> AAIZone::GetEnemies()
{
	return Enemies;
}

const TArray<ALocation*> AAIZone::GetCoverLocations()
{
	return CoverLocations;
}

void AAIZone::AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition)
{
	IStateTreeNotificationsAcceptor::AcceptStateTreeNotification_Implementation(StateTreeNotifier, DataTable, Transition);
	const FName CurrentStateName = UAIUtility::GetCurrentState(StateTreeNotifier, Transition);
	const FName SourceStateName = UAIUtility::GetSourceState(StateTreeNotifier, Transition);
	const FAIZoneManagerMapping* CurrentStateData = DataTable->FindRow<FAIZoneManagerMapping>(CurrentStateName, "");
	const FAIZoneManagerMapping* SourceStateData = DataTable->FindRow<FAIZoneManagerMapping>(SourceStateName, "");

	if(!CurrentStateData || !SourceStateData) return;
	
	CurrentState = CurrentStateData->StateEnum;
	
	StateChanged(SourceStateData->StateEnum, CurrentStateData->StateEnum);
	OnAIZoneManagerStateChangedDelegate.Broadcast(SourceStateData->StateEnum, CurrentStateData->StateEnum);
}

void AAIZone::NotifyPlayerEnteredInSightCone()
{
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
	NumberOfSightConesThePlayerIsIn++;
}

void AAIZone::NotifyPlayerExitedInSightCone()
{
	NumberOfSightConesThePlayerIsIn--;

	if (NumberOfSightConesThePlayerIsIn == 0)
	{
		OnPlayerIsInNoSightConeDelegate.Broadcast();
	}
}

void AAIZone::NotifyPlayerWasSeen()
{
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
}

void AAIZone::NotifyEnemyChangedState(EBasicEnemyState SourceState, EBasicEnemyState NewState)
{
	if (NewState == EBasicEnemyState::Combat)
	{
		StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.DecisionMaking.TriggerCombat")));	
	}
}

// Called every frame
void AAIZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AAIZone::BeginPlay()
{
	Super::BeginPlay();
	
	for (const auto Enemy : Enemies)
	{
		const TObjectPtr<ABasicEnemyController> EnemyController = Cast<ABasicEnemyController>(Enemy->GetController());
		
		EnemyController->OnPlayerEnteredInSightConeDelegate.AddDynamic(this, &AAIZone::NotifyPlayerEnteredInSightCone);
		EnemyController->OnPlayerExitedFromSightConeDelegate.AddDynamic(this, &AAIZone::NotifyPlayerExitedInSightCone);
		EnemyController->OnPlayerSeenDelegate.AddDynamic(this, &AAIZone::NotifyPlayerWasSeen);
		Enemy->OnBasicEnemyStateChangedDelegate.AddDynamic(this, &AAIZone::NotifyEnemyChangedState);
	}
}

