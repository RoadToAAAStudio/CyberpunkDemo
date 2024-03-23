// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"

#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "Components/CapsuleComponent.h"

class AAIZone;

// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicEnemy::NotifySomethingEnteredInTheTrigger);
}

EBasicEnemyState ABasicEnemy::GetCurrentState() const
{
	return CurrentState;
}

TSet<EBasicEnemyBehaviour> ABasicEnemy::GetSupportedBehaviours() const
{
	return SupportedBehaviours;
}

EBasicEnemyBehaviour ABasicEnemy::GetCurrentChosenBehaviour() const
{
	return CurrentChosenBehaviour;
}

// Called when the State Tree notifies a change of state
void ABasicEnemy::AcceptStateTreeNotification_Implementation(const FName& SourceStateName, const FName& CurrentStateName)
{
	IStateTreeNotificationsAcceptor::AcceptStateTreeNotification_Implementation(SourceStateName, CurrentStateName);

	const UEnum* GoalEnum = FindFirstObjectSafe<UEnum>(TEXT("EBasicEnemyState"));
	if (!GoalEnum) return;

	int32 Index = GoalEnum->GetIndexByName(SourceStateName);
	EBasicEnemyState SourceState = Index != INDEX_NONE? static_cast<EBasicEnemyState>(Index) : EBasicEnemyState::None;
	
	Index = GoalEnum->GetIndexByName(CurrentStateName);
	EBasicEnemyState NewState = Index != INDEX_NONE? static_cast<EBasicEnemyState>(Index) : EBasicEnemyState::None;

	CurrentState = NewState;
	
	StateChanged(SourceState, NewState);
	OnBasicEnemyStateChangedDelegate.Broadcast(SourceState, NewState);
}

#pragma region FUNCTIONS_LISTENERS
void ABasicEnemy::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAIZone>(OtherActor))
	{
		AAIZone* AIZone = Cast<AAIZone>(OtherActor);

		if (!BasicEnemyController)
		{
			BasicEnemyController = Cast<ABasicEnemyController>(GetController());
		}
		BasicEnemyController->SharedKnowledge = AIZone;
		BasicEnemyController->SharedKnowledge->OnPlayerIsSensedDelegate.AddDynamic(this, &ABasicEnemy::NotifyPlayerWasSeen);
		BasicEnemyController->SharedKnowledge->OnCombatTimerFinishedDelegate.AddDynamic(this, &ABasicEnemy::NotifyCombatTimerFinished);
		BasicEnemyController->SharedKnowledge->OnAlertedTimerFinishedDelegate.AddDynamic(this, &ABasicEnemy::NotifyAlertedTimerFinished);
	}
}

void ABasicEnemy::NotifyPlayerWasSeen(const ABasicEnemyController* NotifierController)
{
	/*
	 * This listener is called when this actor sees the player or someone else did (via AIZone shared knowledge)
	 * This means that it will be called twice on the actor who actually saw the Player
	 * So the message to the StateTree should be called only once
	 */ 
	
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
}

void ABasicEnemy::NotifyCombatTimerFinished()
{
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.CombatTimerFinished")));
}

void ABasicEnemy::NotifyAlertedTimerFinished()
{
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.AlertedTimerFinished")));
}

#pragma endregion 

#pragma region FUNCTIONS_OVERRIDES
// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Get Supported behaviour from DataTable
	if (SupportedBehavioursDataTable)
	{
		for (auto& RowName : SupportedBehavioursDataTable->GetRowNames())
		{
			const FBasicEnemySupportedBehaviourMapping* SupportedBehaviour = SupportedBehavioursDataTable->FindRow<FBasicEnemySupportedBehaviourMapping>(RowName, "");
			if (!SupportedBehaviour) continue;
			SupportedBehaviours.Append(SupportedBehaviour->BehavioursEnum);
		}
	}
	
	BasicEnemyController = Cast<ABasicEnemyController>(GetController());
	BasicEnemyController->OnPlayerSeenDelegate.AddDynamic(this, &ABasicEnemy::NotifyPlayerWasSeen);
}
#pragma endregion 
