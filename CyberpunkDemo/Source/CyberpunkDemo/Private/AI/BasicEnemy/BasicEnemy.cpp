// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/Utility/AIUtility.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicEnemy::NotifySomethingEnteredInTheTrigger);
}

const AAIZone* ABasicEnemy::GetSharedKnowledge() const
{
	return SharedKnowledge;
}

FVector ABasicEnemy::GetSpawnLocation() const
{
	return SpawnLocation;
}

#pragma region KNOWLEDGE_GETTERS
FGameplayTagContainer ABasicEnemy::GetGameplayTagContainer() const
{
	return GameplayTagContainer;
}

bool ABasicEnemy::HasPatrolGoal() const
{
	return bPatrolGoal;
}

bool ABasicEnemy::HasInvestigationGoal() const
{
	return bInvestigationGoal;
}

bool ABasicEnemy::HasCombatGoal() const
{
	return bCombatGoal;
}

EBasicEnemyState ABasicEnemy::GetCurrentState() const
{
	return CurrentState;
}

const ASplineContainer* ABasicEnemy::GetPatrolSpline() const
{
	return PatrolSpline;
}
#pragma endregion 

// Called when the State Tree notifies a change of state
void ABasicEnemy::AcceptStateTreeNotification_Implementation(const UStateTree* StateTreeNotifier, const UDataTable* DataTable, const FStateTreeTransitionResult& Transition)
{
	IStateTreeNotificationsAcceptor::AcceptStateTreeNotification_Implementation(StateTreeNotifier, DataTable, Transition);
	const FName CurrentStateName = UAIUtility::GetCurrentState(StateTreeNotifier, Transition);
	const FName SourceStateName = UAIUtility::GetSourceState(StateTreeNotifier, Transition);
	const FBasicEnemyStateMapping* CurrentStateData = DataTable->FindRow<FBasicEnemyStateMapping>(CurrentStateName, "");
	const FBasicEnemyStateMapping* SourceStateData = DataTable->FindRow<FBasicEnemyStateMapping>(SourceStateName, "");

	if(!CurrentStateData || !SourceStateData) return;
	
	CurrentState = CurrentStateData->StateEnum;
	
	StateChanged(SourceStateData->StateEnum, CurrentStateData->StateEnum);
	OnBasicEnemyStateChangedDelegate.Broadcast(SourceStateData->StateEnum, CurrentStateData->StateEnum);
}

#pragma region FUNCTIONS_LISTENERS
void ABasicEnemy::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAIZone>(OtherActor))
	{
		AAIZone* AIZone = Cast<AAIZone>(OtherActor);

		// Link BasicEnemy with what it needs
		SharedKnowledge = AIZone;
		SharedKnowledge->OnPlayerIsSensedDelegate.AddDynamic(this, &ABasicEnemy::NotifyPlayerWasSeen);
		SharedKnowledge->OnCombatTimerFinishedDelegate.AddDynamic(this, &ABasicEnemy::NotifyCombatTimerFinished);
		SharedKnowledge->OnAlertedTimerFinishedDelegate.AddDynamic(this, &ABasicEnemy::NotifyAlertedTimerFinished);


		// Link AIZone with what it needs
		SharedKnowledge->Enemies.Add(this);

		const TObjectPtr<ABasicEnemyController> EnemyController = Cast<ABasicEnemyController>(GetController());
		
		EnemyController->OnPlayerEnteredInSightConeDelegate.AddDynamic(SharedKnowledge, &AAIZone::NotifyPlayerEnteredInSightCone);
		EnemyController->OnPlayerExitedFromSightConeDelegate.AddDynamic(SharedKnowledge, &AAIZone::NotifyPlayerExitedInSightCone);
		EnemyController->OnPlayerSeenDelegate.AddDynamic(SharedKnowledge, &AAIZone::NotifyPlayerWasSeen);
	}
}

void ABasicEnemy::NotifyPlayerWasSeen(const ABasicEnemyController* NotifierController)
{
	/*
	 * This listener is called when this actor sees the player or someone else did (via AIZone shared knowledge)
	 * This means that it will be called twice on the actor who actually saw the Player
	 * So the message to the StateTree should be called only once
	 */ 
	
	if (CurrentState == EBasicEnemyState::Combat) return;
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(TEXT("Character.Sensing.Sight.Events.PlayerWasSeen")));
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
	
	BasicEnemyController = Cast<ABasicEnemyController>(GetController());
	BasicEnemyController->OnPlayerSeenDelegate.AddDynamic(this, &ABasicEnemy::NotifyPlayerWasSeen);
}
#pragma endregion 
