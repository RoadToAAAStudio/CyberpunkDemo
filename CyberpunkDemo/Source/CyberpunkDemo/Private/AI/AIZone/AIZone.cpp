// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIZone/AIZone.h"
#include "AI/Utility/AIUtility.h"
#include "AI/WorldInterfacing/Location.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAIZone::AAIZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>("BoxTrigger");
	bGenerateOverlapEventsDuringLevelStreaming = true;
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AAIZone::NotifySomethingEnteredInTheTrigger);
}

const AActor* AAIZone::GetPlayer()
{
	return Player;
}

FTimerHandle AAIZone::GetCombatTimerHandle()
{
	return CombatTimer;
}

FTimerHandle AAIZone::GetAlertedTimerHandle()
{
	return AlertedTimer;
}

int AAIZone::GetNumberOfSightConesThePlayerIsIn()
{
	return NumberOfSightConesThePlayerIsIn;
}

TArray<ABasicEnemy*> AAIZone::GetEnemies()
{
	return Enemies;
}

TArray<ALocation*> AAIZone::GetCoverLocations()
{
	return CoverLocations;
}

EAIZoneState AAIZone::GetCurrentState()
{
	return CurrentState;
}

void AAIZone::AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition)
{
	IStateTreeNotificationsAcceptor::AcceptStateTreeNotification_Implementation(StateTreeNotifier, DataTable, Transition);
	const FName CurrentStateName = UAIUtility::GetCurrentState(StateTreeNotifier, Transition);
	const FName SourceStateName = UAIUtility::GetSourceState(StateTreeNotifier, Transition);
	const FAIZoneMapping* CurrentStateData = DataTable->FindRow<FAIZoneMapping>(CurrentStateName, "");
	const FAIZoneMapping* SourceStateData = DataTable->FindRow<FAIZoneMapping>(SourceStateName, "");

	if(!CurrentStateData || !SourceStateData) return;
	
	CurrentState = CurrentStateData->StateEnum;
	
	switch (SourceStateData->StateEnum)
	{
	case EAIZoneState::Unaware:
		break;
		
	case EAIZoneState::Combat:
		Player = nullptr;
		break;
		
	case EAIZoneState::Alerted:
		break;
		
	case EAIZoneState::Max:
		break;
		
	}
	
	switch (CurrentState)
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
				StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.AlertedTimerFinished")));
				AlertedTimerFinished();
				OnAlertedTimerFinishedDelegate.Broadcast();
			});

			GetWorld()->GetTimerManager().SetTimer(AlertedTimer, TimerCallback, 4.0f, false);
			AlertedTimerStarted();
			OnAlertedTimerStartedDelegate.Broadcast();
		}
		break;
		
	case EAIZoneState::Max:
		break;
		
	}
	
	StateChanged(SourceStateData->StateEnum, CurrentStateData->StateEnum);
	OnAIZoneManagerStateChangedDelegate.Broadcast(SourceStateData->StateEnum, CurrentStateData->StateEnum);
}

// Something entered the BoxTrigger
void AAIZone::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABasicEnemy>(OtherActor))
	{
		ABasicEnemy* Enemy = Cast<ABasicEnemy>(OtherActor);
		Enemies.Add(Enemy);

		const TObjectPtr<ABasicEnemyController> EnemyController = Cast<ABasicEnemyController>(Enemy->GetController());
		
		EnemyController->OnPlayerEnteredInSightConeDelegate.AddDynamic(this, &AAIZone::NotifyPlayerEnteredInSightCone);
		EnemyController->OnPlayerExitedFromSightConeDelegate.AddDynamic(this, &AAIZone::NotifyPlayerExitedInSightCone);
		EnemyController->OnPlayerSeenDelegate.AddDynamic(this, &AAIZone::NotifyPlayerWasSeen);
	}
	else if (Cast<ALocation>(OtherActor))
	{
		CoverLocations.Add(Cast<ALocation>(OtherActor));
	}
}

// Player entered a sight cone
void AAIZone::NotifyPlayerEnteredInSightCone()
{
	NumberOfSightConesThePlayerIsIn++;

	if (NumberOfSightConesThePlayerIsIn == 1)
	{
		if (CurrentState == EAIZoneState::Combat)
		{
			GetWorld()->GetTimerManager().ClearTimer(CombatTimer);
		}
	}
}

// Player exited from a sight cone
void AAIZone::NotifyPlayerExitedInSightCone()
{
	NumberOfSightConesThePlayerIsIn--;

	if (NumberOfSightConesThePlayerIsIn == 0)
	{
		if (CurrentState == EAIZoneState::Combat)
		{
			// Start Combat Timer
			FTimerDelegate TimerCallback = FTimerDelegate::CreateLambda([this]()
			{
				StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.CombatTimerFinished")));
				CombatTimerFinished();
				OnCombatTimerFinishedDelegate.Broadcast();
			});

			GetWorld()->GetTimerManager().SetTimer(CombatTimer, TimerCallback,4.0f, false);
			CombatTimerStarted();
			OnCombatTimerStartedDelegate.Broadcast();
		}
		PlayerIsInNoSightCone();
		OnPlayerIsInNoSightConeDelegate.Broadcast();
	}
}

// Player was seen by a BasicEnemy
void AAIZone::NotifyPlayerWasSeen()
{
	if (CurrentState == EAIZoneState::Unaware || CurrentState == EAIZoneState::Alerted)
	{
		if (CurrentState == EAIZoneState::Alerted)
		{
			GetWorld()->GetTimerManager().ClearTimer(AlertedTimer);
		}
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
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
}