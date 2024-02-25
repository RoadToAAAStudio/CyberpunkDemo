// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/BasicEnemyController.h"

#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ABasicEnemyController::ABasicEnemyController(const FObjectInitializer& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ABasicEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameplayTagsContainer.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.PlayerInCone"))))
	{
		SightBar->AddAmount(SightIncreaseRate * DeltaTime);
	}
	else
	{
		SightBar->RemoveAmount(SightDecreaseRate * DeltaTime);
	}

	if (HearingBar->CurrentValue > 0)
	{
		HearingBar->RemoveAmount(HearingDecreaseRate * DeltaTime);
	}
}


void ABasicEnemyController::SetupPerceptionSystem()
{
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	if (HearingConfig)
	{
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);
	}

	SightBar = CreateDefaultSubobject<UAttributeBar>(TEXT("SightBar"));
	HearingBar = CreateDefaultSubobject<UAttributeBar>(TEXT("HearingBar"));
}

void ABasicEnemyController::ReceiveStimulus(AActor* Actor, const FAIStimulus Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0:
		if (Actor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				GameplayTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.PlayerInCone")));
				PlayerEnteredInSightCone();
				OnPlayerEnteredInSightCone.Broadcast();
			}
			else
			{
				GameplayTagsContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.PlayerInCone")));
				PlayerExitedFromSightCone();
				OnPlayerExitedFromSightCone.Broadcast();
			}
		}
		break;
	case 1:
		//TODO It depends on the Stimulus
		HearingBar->AddAmount(1.0f);
		if (HearingBar->bIsFull)
		{
			SomethingWasHeard(Stimulus);
			OnHeardSomethingDelegate.Broadcast(Stimulus);
		}

		break;
	default:
		break;
	}
}

void ABasicEnemyController::ReceivePlayerSeen()
{
	GameplayTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.PlayerSeen")));
}

void ABasicEnemyController::BeginPlay()
{
	Super::BeginPlay();

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicEnemyController::ReceiveStimulus);
	SightBar->OnBarFilledDelegate.AddDynamic(this, &ABasicEnemyController::ReceivePlayerSeen);
}

void ABasicEnemyController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
}
