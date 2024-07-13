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

// Sets default values
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
		APawn* Player = SharedKnowledge.Player.Get();
		if (Player)
		{
			SharedKnowledge.PlayerLocation = Player->GetActorLocation();
		}
	}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (bDebugSharedKnowledge)
	{
		DebugSharedKnowledge();
	}
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
		if (Cast<ALocation>(actor))
		{
			ALocation* Location = Cast<ALocation>(actor);
			SharedKnowledge.CoverPerLocations.Add(Location->GetActorLocation(), Location);
		}
		else if (Cast<ABasicEnemy>(actor))
		{
			ABasicEnemy* Enemy = Cast<ABasicEnemy>(actor);
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

void AAIZone::DebugSharedKnowledge() const
{
	// Debug Name
	{
		const FString String = FString::Printf(TEXT("LocalNetwork Name: %s----------------------------------------------"), *GetActorNameOrLabel());
		PRINT_ON_SCREEN(0.0f, FColor::Yellow, String, false);
	}

	// Debug Shared Knowledge
	{
		FString String = FString::Printf(TEXT("[SHARED KNOWLEDGE]\n"));

		const FSettablePawn& Player = SharedKnowledge.Player;
		const bool bIsPlayerSet = Player.IsSet();
		const FSettableVector& PlayerLocation = SharedKnowledge.PlayerLocation;
		String += FString::Printf(TEXT("Player (%s): %s - Location (%s): (%.2f, %.2f, %.2f)\n"), Player.IsSet() ? "*" : "-", bIsPlayerSet ? *(Player.Get()->GetActorNameOrLabel()) : TEXT("null"), PlayerLocation.IsSet() ? "*" : "-", PlayerLocation.Get().X, PlayerLocation.Get().Y, PlayerLocation.Get().Z);

		const FSettableTimerHandle& CombatTimer = SharedKnowledge.CombatTimer;
		const FSettableTimerHandle& AlertedTimer = SharedKnowledge.AlertedTimer;
		String += FString::Printf(TEXT("CombatTimer (%s): %.2f - AlertedTimer (%s): %.2f\n"), CombatTimer.IsSet() ? "*" : "-", GetWorldTimerManager().GetTimerRemaining(CombatTimer.Get()), AlertedTimer.IsSet() ? "*" : "-", GetWorldTimerManager().GetTimerRemaining(AlertedTimer.Get()));

		const FSettableInt& NumberOfEnemiesConePlayerIsIn = SharedKnowledge.NumberOfSightConesThePlayerIsIn;
		String += FString::Printf(TEXT("Enemies (player is in %d enemies' sight cone)\n"), NumberOfEnemiesConePlayerIsIn.Get());

		const TMap<FVector, TObjectPtr<ALocation>>& CoverPerLocations = SharedKnowledge.CoverPerLocations;
		String += FString::Printf(TEXT("Covers: %d\n"), CoverPerLocations.Num());

		const EAIZoneState State = SharedKnowledge.AIZoneState;
		String += FString::Printf(TEXT("State: %s"), *(UEnum::GetDisplayValueAsText(State).ToString()));

		PRINT_ON_SCREEN(0.0f, FColor(0, 140, 255, 255), String, false);
	}
}

void AAIZone::DebugEnemies() const
{
	for (auto& Enemy : SharedKnowledge.Enemies)
	{
		FVector EnemyLocation = Enemy->GetActorLocation();
		DRAW_LINE(GetActorLocation(), EnemyLocation, FColor::Yellow, -1.0f);
		DRAW_STRING(EnemyLocation + FVector::UpVector * 250.0f, Enemy->GetActorNameOrLabel(), FColor::Yellow, 0.0f);
	}
}

void AAIZone::DebugCovers() const
{
	for (auto& CoverData : SharedKnowledge.CoverPerLocations)
	{
		FVector CoverLocation = CoverData.Key;
		DRAW_SPHERE(CoverLocation, 25.0f, 4, FColor::Yellow, -1.0f);
	}
}

#pragma region FUNCTIONS_LISTENERS
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