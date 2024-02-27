// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Enemies/BasicEnemy.h"

#include "AI/AIUtility.h"

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

	if(!CurrentStateData || ! SourceStateData) return;
	
	CurrentState = CurrentStateData->StateEnum;
	StateChanged(SourceStateData->StateEnum, CurrentStateData->StateEnum);
	OnBasicEnemyStateChangedDelegate.Broadcast(SourceStateData->StateEnum, CurrentStateData->StateEnum);
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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

