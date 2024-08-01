// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "AI/BasicEnemy/Knowledge/BasicEnemyKnowledgeComponent.h"
#include "AI/WorldInterfacing/Location.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StateTreeComponent.h"
#include "CyberpunkDemo/DebugMacros.h"

AAIZone::AAIZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateMachine = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>("BoxTrigger");
}

void AAIZone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Update Shared Knowledge
	{
		if (SharedKnowledge.Player.IsSet())
		{
			SharedKnowledge.PlayerLocation = SharedKnowledge.Player.Get()->GetActorLocation();
		}
	}

	// Update Debug
	{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (bDebugEnemies)
		{
			DebugEnemies();
		}
		if (bDebugCovers)
		{
			DebugCovers();
		}
#endif
	}
}

void AAIZone::GetChangeOfState_Implementation(const FName& SourceStateName, const FName& CurrentStateName)
{
	const UEnum* GoalEnum = FindFirstObjectSafe<UEnum>(TEXT("EAIZoneState"));
	if (!GoalEnum) return;

	int32 Index = GoalEnum->GetIndexByName(SourceStateName);
	EAIZoneState SourceState = Index != INDEX_NONE ? static_cast<EAIZoneState>(Index) : EAIZoneState::None;
	
	Index = GoalEnum->GetIndexByName(CurrentStateName);
	EAIZoneState NewState = Index != INDEX_NONE ? static_cast<EAIZoneState>(Index) : EAIZoneState::None;
	
	SharedKnowledge.AIZoneState = NewState;
	
	switch (SourceState)
	{
		case EAIZoneState::Unaware:
			break;
			
		case EAIZoneState::Combat:
		{
			SharedKnowledge.Player.UnSet();
			SharedKnowledge.PlayerLocation.UnSet();
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
				SharedKnowledge.AlertedTimer.UnSet();
				StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.AlertedTimerFinished")));
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

	SharedKnowledge.CombatTimerDuration.Set(CombatTimerDuration);
	SharedKnowledge.AlertedTimerDuration.Set(AlertedTimerDuration);
	SharedKnowledge.NumberOfSightConesThePlayerIsIn.Set(0);

	RegisterActors();
}

void AAIZone::RegisterActors()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); // BasicEnemy
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2)); // Covers
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(this, 1);
	
	TArray<AActor*> overlappingActors;
	UKismetSystemLibrary::BoxOverlapActors(this->GetWorld(), BoxTrigger->GetComponentLocation(), BoxTrigger->GetScaledBoxExtent(), traceObjectTypes, nullptr, ignoreActors, overlappingActors);

	for (int i = 0; i < overlappingActors.Num(); i++)
	{
		AActor* actor = overlappingActors[i];
		if (ALocation* Location = Cast<ALocation>(actor))
		{
			SharedKnowledge.CoverPerLocations.Add(Location->GetActorLocation(), Location);
		}
		else if (ABasicEnemy* Enemy = Cast<ABasicEnemy>(actor))
		{
			Enemy->RegisterAIZone(this);
			SharedKnowledge.Enemies.AddUnique(Enemy);
			
			ABasicEnemyController* EnemyController = Cast<ABasicEnemyController>(Enemy->GetController());
			if (EnemyController)
			{
				EnemyController->KnowledgeComponent->OnPlayerEnteredSightConeDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerEnteredInSightCone);
				EnemyController->KnowledgeComponent->OnPlayerExitedSightConeDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerExitedInSightCone);
				EnemyController->KnowledgeComponent->OnPlayerSeenDelegate.AddUniqueDynamic(this, &AAIZone::NotifyPlayerWasSeen);
			}
		}
	}
}

void AAIZone::DebugEnemies() const
{
	for (auto& Enemy : SharedKnowledge.Enemies)
	{
		FVector EnemyLocation = Enemy->GetActorLocation();
		DRAW_LINE(this, GetActorLocation(), EnemyLocation, FColor::Yellow, -1.0f);
		DRAW_STRING(this, EnemyLocation + FVector::UpVector * 250.0f, Enemy->GetActorNameOrLabel(), FColor::Yellow, 0.0f);
	}
}

void AAIZone::DebugCovers() const
{
	for (auto& CoverData : SharedKnowledge.CoverPerLocations)
	{
		FVector CoverLocation = CoverData.Key;
		DRAW_SPHERE(this, CoverLocation, 25.0f, 4, FColor::Yellow, -1.0f);
	}
}

void AAIZone::NotifyPlayerEnteredInSightCone(const APawn* PawnOwner)
{
	SharedKnowledge.NumberOfSightConesThePlayerIsIn.Set(SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get() + 1);

	if (SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get() == 1)
	{
		if (SharedKnowledge.AIZoneState == EAIZoneState::Combat)
		{
			SharedKnowledge.CombatTimer.UnSet();
			GetWorld()->GetTimerManager().ClearTimer(SharedKnowledge.CombatTimer.Get());
		}
	}
}

void AAIZone::NotifyPlayerExitedInSightCone(const APawn* PawnOwner)
{
	SharedKnowledge.NumberOfSightConesThePlayerIsIn.Set(SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get() - 1);
 
	if (SharedKnowledge.NumberOfSightConesThePlayerIsIn.Get() > 0) return;

	if (SharedKnowledge.AIZoneState == EAIZoneState::Combat)
	{
		FTimerDelegate TimerCallback = FTimerDelegate::CreateLambda([this]()
		{
			SharedKnowledge.CombatTimer.UnSet();
			StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.CombatTimerFinished")));
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
		SharedKnowledge.Player.Set(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
		PlayerSensed();
		OnPlayerIsSensedDelegate.Broadcast(PawnOwner);
	}
}