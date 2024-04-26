// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BasicEnemy/BasicEnemy.h"

#include "AI/Actuation/SettableStateTreeComponent.h"
#include "AI/AIZone/AIZone.h"
#include "AI/BasicEnemy/BasicEnemyController.h"
#include "Components/CapsuleComponent.h"

// Sets default values
// ABasicEnemy::ABasicEnemy()
// {
//  	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
// 	PrimaryActorTick.bCanEverTick = true;
//
// 	StateMachine = CreateDefaultSubobject<UStateTreeComponent>(TEXT("StateTree"));
// 	CurrentBehaviour = CreateDefaultSubobject<USettableStateTreeComponent>(TEXT("Behaviour"));
// 	
// 	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicEnemy::NotifySomethingEnteredInTheTrigger);
// }
//
// ABasicEnemyController* ABasicEnemy::GetBasicEnemyController() const
// {
// 	return BasicEnemyController;
// }
//
// TSet<EBasicEnemyBehaviour> ABasicEnemy::GetSupportedBehaviours() const
// {
// 	TSet<EBasicEnemyBehaviour> Behaviours;
// 	SupportedBehaviours.GetKeys(Behaviours);
// 	return Behaviours;
// }
//
// EBasicEnemyBehaviour ABasicEnemy::GetCurrentChosenBehaviour() const
// {
// 	return ChosenBehaviour;
// }
//
// void ABasicEnemy::SelectBehaviour()
// {
// 		// See if a new Behaviour must be selected
// 	TSet<EBasicEnemyBehaviour> SupportedBehavioursSet;
// 	SupportedBehaviours.GetKeys(SupportedBehavioursSet);
// 	TSet<EBasicEnemyGoal> GeneratedGoals = BasicEnemyController->GetGeneratedGoals();
//
//
// 	// Binary Filter
// 	
// 	// The possible behaviour are filtered by state
// 	switch (CurrentState)
// 	{
// 		case EBasicEnemyState::Unaware:
// 		{
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::BlindInvestigation);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Shoot);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::QuickMeleeAttack);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ThrowGrenade);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::MoveToCover);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ShootFromCover);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ThrowGrenadeFromCover);
// 		}
// 		break;	
// 		
// 		case EBasicEnemyState::Combat:
// 		{
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Idle);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ReturnToSpawnPoint);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Patrol);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::BlindInvestigation);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Investigation);
// 		}
// 		break;
// 		
// 		case EBasicEnemyState::Alerted:
// 		{
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Idle);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ReturnToSpawnPoint);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Patrol);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Shoot);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::QuickMeleeAttack);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ThrowGrenade);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::MoveToCover);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ShootFromCover);
// 			SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ThrowGrenadeFromCover);
// 		}
// 		break;
// 	}
//
// 	// Other filtering
//
// 	float DistanceFormPlayer = BasicEnemyController->GetDistanceFromPlayer();
// 	
// 	// Filter Return To SpawnPoint
// 	if (FVector::Distance(GetActorLocation(), BasicEnemyController->GetSpawnLocation()) < 5.0f)
// 	{
// 		SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::ReturnToSpawnPoint);
// 	}
//
// 	// Filter Patrol
// 	if (!GeneratedGoals.Contains(EBasicEnemyGoal::Patrol))
// 	{
// 		SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Patrol);
// 	}
//
// 	// Filter Investigations
// 	if (!GeneratedGoals.Contains(EBasicEnemyGoal::Search))
// 	{
// 		SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::BlindInvestigation);
// 		SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Investigation);
// 	}
//
// 	// Filter Shoot
// 	if (!GeneratedGoals.Contains(EBasicEnemyGoal::Combat) ||
// 		      DistanceFormPlayer > MaxDistanceToShoot ||
// 		      DistanceFormPlayer < MinDistanceToShoot)
// 	{
// 		SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::Shoot);
// 	}
//
// 	// Filter QuickMelee Attack
// 	if (!GeneratedGoals.Contains(EBasicEnemyGoal::Combat) ||
// 		  DistanceFormPlayer > MaxDistanceToQuickMeleeAttack)
// 	{
// 		SupportedBehavioursSet.Remove(EBasicEnemyBehaviour::QuickMeleeAttack);
// 	}
// 	
// 	// TODO Other Filtering
//
// 	EBasicEnemyBehaviour BestBehaviour = EBasicEnemyBehaviour::None;
// 	
// 	// Check results
// 	if (SupportedBehavioursSet.Num() == 0)
// 	{
// 		// Default behaviour if nothing is possible
// 		BestBehaviour = EBasicEnemyBehaviour::None;
// 	}
// 	else if (SupportedBehavioursSet.Num() == 1)
// 	{
// 		// Trivial case
// 		for (const auto& Behaviour : SupportedBehavioursSet)
// 		{
// 			BestBehaviour = Behaviour;
// 			break;
// 		}
// 	}
// 	else
// 	{
// 		// Assign to each one a score
// 		int BestScore = -1;
//
// 		for (const auto& Behaviour : SupportedBehavioursSet)
// 		{
// 			int BehaviourScore = -1;
// 			switch (Behaviour)
// 			{
// 			case EBasicEnemyBehaviour::Idle:
// 				BehaviourScore = 1;
// 				break;
// 			case EBasicEnemyBehaviour::ReturnToSpawnPoint:
// 				BehaviourScore = 10;
// 				break;
// 			case EBasicEnemyBehaviour::Patrol:
// 				BehaviourScore = 20;
// 				break;
// 			case EBasicEnemyBehaviour::BlindInvestigation:
// 				BehaviourScore = 30;
// 				break;
// 			case EBasicEnemyBehaviour::Investigation:
// 				BehaviourScore = 40;
// 				break;
// 			case EBasicEnemyBehaviour::Shoot:
// 				BehaviourScore = 50;
// 				break;
// 			case EBasicEnemyBehaviour::QuickMeleeAttack:
// 				BehaviourScore = 60;
// 				break;
// 			case EBasicEnemyBehaviour::ThrowGrenade:
// 				BehaviourScore = 70;
// 				break;
// 			case EBasicEnemyBehaviour::MoveToCover:
// 				BehaviourScore = 80;
// 				break;
// 			case EBasicEnemyBehaviour::ShootFromCover:
// 				BehaviourScore = 90;
// 				break;
// 			case EBasicEnemyBehaviour::ThrowGrenadeFromCover:
// 				BehaviourScore = 100;
// 				break;
// 			}
//
// 			if (BehaviourScore > BestScore)
// 			{
// 				BestBehaviour = Behaviour;
// 				BestScore = BehaviourScore;
// 			}
// 		}
// 	}
//
// 	ChosenBehaviour = BestBehaviour;
//
// 	// Prepare Data
// 		switch (ChosenBehaviour)
// 		{
// 			case EBasicEnemyBehaviour::Idle:
// 				break;
// 			case EBasicEnemyBehaviour::ReturnToSpawnPoint:
// 				break;
// 		case EBasicEnemyBehaviour::Patrol:
// 				ActuatorsData.Spline = Cast<USplineComponent>(PatrolSpline->GetComponentByClass(USplineComponent::StaticClass()));
// 				break;
// 			case EBasicEnemyBehaviour::BlindInvestigation:
// 				break;
// 			case EBasicEnemyBehaviour::Investigation:
// 				break;
// 			case EBasicEnemyBehaviour::Shoot:
// 				break;
// 			case EBasicEnemyBehaviour::QuickMeleeAttack:
// 				break;
// 			case EBasicEnemyBehaviour::ThrowGrenade:
// 				break;
// 			case EBasicEnemyBehaviour::MoveToCover:
// 				break;
// 			case EBasicEnemyBehaviour::ShootFromCover:
// 				break;
// 			case EBasicEnemyBehaviour::ThrowGrenadeFromCover:
// 				break;
// 		}
// 	
// 	// Try to run
// 	FBasicEnemySupportedBehaviourMapping* Behaviour = SupportedBehavioursDataTable->FindRow<FBasicEnemySupportedBehaviourMapping>(UEnum::GetValueAsName(ChosenBehaviour), "");
// 	if (Behaviour && Behaviour->BehaviourAsset)
// 	{
// 		CurrentBehaviour->SetTree(Behaviour->BehaviourAsset);
// 	}
// }
//
// #pragma region FUNCTIONS_LISTENERS
// void ABasicEnemy::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	if (Cast<AAIZone>(OtherActor))
// 	{
// 		AAIZone* AIZone = Cast<AAIZone>(OtherActor);
//
// 		if (!BasicEnemyController)
// 		{
// 			BasicEnemyController = Cast<ABasicEnemyController>(GetController());
// 		}
// 		BasicEnemyController->SharedKnowledge = AIZone;
// 		BasicEnemyController->SharedKnowledge->OnPlayerIsSensedDelegate.AddDynamic(this, &ABasicEnemy::NotifyPlayerWasSeen);
// 		BasicEnemyController->SharedKnowledge->OnCombatTimerFinishedDelegate.AddDynamic(this, &ABasicEnemy::NotifyCombatTimerFinished);
// 		BasicEnemyController->SharedKnowledge->OnAlertedTimerFinishedDelegate.AddDynamic(this, &ABasicEnemy::NotifyAlertedTimerFinished);
// 	}
// }
//
// void ABasicEnemy::NotifyPlayerWasSeen(const ABasicEnemyController* NotifierController)
// {
// 	/*
// 	 * This listener is called when this actor sees the player or someone else did (via AIZone shared knowledge)
// 	 * This means that it will be called twice on the actor who actually saw the Player
// 	 * So the message to the StateTree should be called only once
// 	 */ 
// 	
// 	StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.PlayerWasSeen")));
// }
//
// void ABasicEnemy::NotifyCombatTimerFinished()
// {
// 	StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.CombatTimerFinished")));
// }
//
// void ABasicEnemy::NotifyAlertedTimerFinished()
// {
// 	StateMachine->SendStateTreeEvent(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.Events.AlertedTimerFinished")));
// }
//
// void ABasicEnemy::NotifyGoalGenerated(const TSet<EBasicEnemyGoal> NewGoals, const TSet<EBasicEnemyGoal> RemovedGoals)
// {
// 	SelectBehaviour();
// }
//
// #pragma endregion 
//
// #pragma region FUNCTIONS_OVERRIDES
// // Called every frame
// void ABasicEnemy::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
// }
//
// // Called to bind functionality to input
// void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// }
//
// // Called when the game starts or when spawned
// void ABasicEnemy::BeginPlay()
// {
// 	Super::BeginPlay();
// 	
// 	// Get Supported behaviour from DataTable
// 	if (SupportedBehavioursDataTable)
// 	{
// 		for (auto& RowName : SupportedBehavioursDataTable->GetRowNames())
// 		{
// 			const FBasicEnemySupportedBehaviourMapping* SupportedBehaviour = SupportedBehavioursDataTable->FindRow<FBasicEnemySupportedBehaviourMapping>(RowName, "");
// 			if (!SupportedBehaviour) continue;
// 			SupportedBehaviours.Add(SupportedBehaviour->BehaviourEnum, SupportedBehaviour->BehaviourAsset);
// 		}
// 	}
// 	
// 	BasicEnemyController = Cast<ABasicEnemyController>(GetController());
// 	BasicEnemyController->OnPlayerSeenDelegate.AddDynamic(this, &ABasicEnemy::NotifyPlayerWasSeen);
// 	BasicEnemyController->OnGoalsChanged.AddDynamic(this, &ABasicEnemy::NotifyGoalGenerated);
//
// 	// Initialize Behaviour
// 	SelectBehaviour();
// }
// #pragma endregion

void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();

	BasicEnemyController = Cast<ABasicEnemyController>(GetController());
	if (BasicEnemyController)
	{
		BasicEnemyController->Initialize(this);
	}
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABasicEnemy::NotifySomethingEnteredInTheTrigger);
}

#pragma region FUNCTIONS_LISTENERS
void ABasicEnemy::NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AAIZone>(OtherActor))
	{
		this->AIZone = Cast<AAIZone>(OtherActor);
	}
}
#pragma endregion 
