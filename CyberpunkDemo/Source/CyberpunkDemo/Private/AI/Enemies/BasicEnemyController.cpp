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
	
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicEnemyController::OnTargetDetected);
}

void ABasicEnemyController::OnTargetDetected(AActor* Actor, const FAIStimulus Stimulus)
{
	switch (Stimulus.Type)
	{
	case 0:
		if (Actor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				UE_LOG(LogTemp, Display, TEXT("MainCharacter seen!"));
				PlayerSeen();
				OnPlayerSeenDelegate.Broadcast();
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("MainCharacter lost!"));
				PlayerLost();
				OnPlayerLostDelegate.Broadcast();
			}
		}
		break;
	case 1:
		UE_LOG(LogTemp, Display, TEXT("Heard something!"));
		SomethingWasHeard(Stimulus);
		OnHeardSomethingDelegate.Broadcast(Stimulus);
		break;
	default:
		break;
	}
}

void ABasicEnemyController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
}
