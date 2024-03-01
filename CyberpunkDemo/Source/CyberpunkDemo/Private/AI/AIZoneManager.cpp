// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIZoneManager.h"
#include "AI/AIUtility.h"

// Sets default values
AAIZoneManager::AAIZoneManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}

void AAIZoneManager::AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition)
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

void AAIZoneManager::NotifyChildrenEnemies(const FGameplayTag& GameplayTag)
{
	for (const auto Enemy : Enemies)
	{
		Enemy->ReceiveTriggerEvent(GameplayTag);
	}
}

void AAIZoneManager::NotifyPlayerEnteredInSightCone()
{
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
	NumberOfConesPlayerIsIn++;
}

void AAIZoneManager::NotifyPlayerExitedInSightCone()
{
	NumberOfConesPlayerIsIn--;

	if (NumberOfConesPlayerIsIn == 0)
	{
		OnPlayerIsInNoSightConeDelegate.Broadcast();
	}
}

void AAIZoneManager::NotifyPlayerWasSeen()
{
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
}

void AAIZoneManager::NotifyEnemyChangedState(EBasicEnemyState SourceState, EBasicEnemyState NewState)
{
	if (NewState == EBasicEnemyState::Combat)
	{
		StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.DecisionMaking.TriggerCombat")));	
	}
}

// Called every frame
void AAIZoneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AAIZoneManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (const auto Enemy : Enemies)
	{
		const TObjectPtr<ABasicEnemyController> EnemyController = Cast<ABasicEnemyController>(Enemy->GetController());
		
		EnemyController->OnPlayerEnteredInSightConeDelegate.AddDynamic(this, &AAIZoneManager::NotifyPlayerEnteredInSightCone);
		EnemyController->OnPlayerExitedFromSightConeDelegate.AddDynamic(this, &AAIZoneManager::NotifyPlayerExitedInSightCone);
		EnemyController->OnPlayerSeenDelegate.AddDynamic(this, &AAIZoneManager::NotifyPlayerWasSeen);
		Enemy->OnBasicEnemyStateChangedDelegate.AddDynamic(this, &AAIZoneManager::NotifyEnemyChangedState);
	}
}

