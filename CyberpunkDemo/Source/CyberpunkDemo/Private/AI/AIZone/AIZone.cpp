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

#pragma region SHARED_KNOWLEDGE_GETTERS
const AActor* AAIZone::GetPlayer() const
{
	return Player;
}

FTimerHandle AAIZone::GetCombatTimerHandle() const
{
	return CombatTimer;
}

FTimerHandle AAIZone::GetAlertedTimerHandle() const
{
	return AlertedTimer;
}

int AAIZone::GetNumberOfSightConesThePlayerIsIn() const
{
	return NumberOfSightConesThePlayerIsIn;
}

TArray<ABasicEnemy*> AAIZone::GetEnemies() const
{
	TArray<ABasicEnemy*> Copy = Enemies;
	return Copy;
}

TArray<ALocation*> AAIZone::GetCoverLocations() const
{
	TArray<ALocation*> Copy = CoverLocations;
	return Copy;
}

EAIZoneState AAIZone::GetCurrentState() const
{
	return CurrentState;
}
#pragma endregion 

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
		PlayerIsForgotten();
		OnPlayerIsForgottenDelegate.Broadcast();
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

#pragma region FUNCTIONS_LISTENERS
// Something entered the BoxTrigger
void AAIZone::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ALocation>(OtherActor))
	{
		CoverLocations.Add(Cast<ALocation>(OtherActor));
	}
}

// Player entered a sight cone
void AAIZone::NotifyPlayerEnteredInSightCone(const ABasicEnemyController* Controller)
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
void AAIZone::NotifyPlayerExitedInSightCone(const ABasicEnemyController* Controller)
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
void AAIZone::NotifyPlayerWasSeen(const ABasicEnemyController* Controller)
{
	if (CurrentState == EAIZoneState::Unaware || CurrentState == EAIZoneState::Alerted)
	{
		if (CurrentState == EAIZoneState::Alerted)
		{
			GetWorld()->GetTimerManager().ClearTimer(AlertedTimer);
		}
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		PlayerIsSensed();
		OnPlayerIsSensedDelegate.Broadcast(Controller);
		StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
	}
}
#pragma endregion 

#pragma region FUNCTIONS_OVERRIDES
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
#pragma endregion 