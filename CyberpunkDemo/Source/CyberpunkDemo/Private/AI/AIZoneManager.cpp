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

void AAIZoneManager::NotifyPlayerWasSeen()
{
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsSeen")));
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
		Enemy->BasicEnemyController->OnPlayerSeenDelegate.AddDynamic(this, &AAIZoneManager::NotifyPlayerWasSeen);
	}
}

