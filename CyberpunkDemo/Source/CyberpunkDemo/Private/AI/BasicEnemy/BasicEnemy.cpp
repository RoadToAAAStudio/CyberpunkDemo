// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"
#include "AI/Utility/AIUtility.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
ABasicEnemy::ABasicEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
}

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

	switch (CurrentState)
	{
	case EBasicEnemyState::Unaware:
		BasicEnemyController->BrainComponent->StopLogic(FString(""));
		BasicEnemyController->RunBehaviorTree(BTUnaware);
		break;
	case EBasicEnemyState::Combat:
		BasicEnemyController->BrainComponent->StopLogic(FString(""));
		BasicEnemyController->RunBehaviorTree(BTCombat);
		break;
	case EBasicEnemyState::Alerted:
		BasicEnemyController->BrainComponent->StopLogic(FString(""));
		BasicEnemyController->RunBehaviorTree(BTAlerted);
		break;
	case EBasicEnemyState::Max:
		break;
	default:
		break;
	}
	
	StateChanged(SourceStateData->StateEnum, CurrentStateData->StateEnum);
	OnBasicEnemyStateChangedDelegate.Broadcast(SourceStateData->StateEnum, CurrentStateData->StateEnum);
}

void ABasicEnemy::ReceiveTriggerEvent(const FGameplayTag& GameplayTag) const
{
	StateTree->SendStateTreeEvent(GameplayTag);
}

void ABasicEnemy::NotifyPlayerWasSeen()
{
	StateTree->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(TEXT("Character.Sensing.Sight.PlayerIsSeen")));
}

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
	BasicEnemyController->RunBehaviorTree(BTUnaware);
	BasicEnemyController->OnPlayerSeenDelegate.AddDynamic(this, &ABasicEnemy::NotifyPlayerWasSeen);

	if (PatrolSpline)
	{
		BasicEnemyController->GetBlackboardComponent()->SetValueAsObject("PatrolSpline", PatrolSpline);
	}
}
