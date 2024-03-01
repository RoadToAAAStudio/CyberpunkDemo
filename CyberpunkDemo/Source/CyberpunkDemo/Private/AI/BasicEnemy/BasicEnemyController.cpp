// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BasicEnemy/BasicEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ABasicEnemyController::ABasicEnemyController(const FObjectInitializer& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void ABasicEnemyController::EnableSightSense(bool Enable)
{
	if (Enable)
	{
		GameplayTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
		SightBar->OnBarFilledDelegate.AddDynamic(this, &ABasicEnemyController::SightBarFull);
		SightBar->Reset();

		// TODO What if the player is already in the cone?

		OnSightSenseToggledDelegate.Broadcast(true);
	}
	else
	{
		bool a = GameplayTagsContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
		bool b = GameplayTagsContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
		bool c = GameplayTagsContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
		SightBar->OnBarFilledDelegate.RemoveDynamic(this, &ABasicEnemyController::SightBarFull);
		SightBar->Reset();

		UE_LOG(LogTemp, Warning, TEXT("%d"), a);
		UE_LOG(LogTemp, Warning, TEXT("%d"), b);
		UE_LOG(LogTemp, Warning, TEXT("%d"), c);
		
		OnSightSenseToggledDelegate.Broadcast(false);
	}
}

void ABasicEnemyController::EnableHearingSense(bool Enable)
{
	if (Enable)
	{
		GameplayTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
		HearingBar->OnBarFilledDelegate.AddDynamic(this, &ABasicEnemyController::HearingBarFull);
		HearingBar->Reset();

		OnHearingSenseToggledDelegate.Broadcast(true);
	}
	else
	{
		GameplayTagsContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
		HearingBar->OnBarFilledDelegate.RemoveDynamic(this, &ABasicEnemyController::HearingBarFull);
		HearingBar->Reset();
		
		OnHearingSenseToggledDelegate.Broadcast(false);
	}
}

bool ABasicEnemyController::IsSightEnabled()
{
	return GameplayTagsContainer.HasTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight")));
}

bool ABasicEnemyController::IsHearingEnabled()
{
	return GameplayTagsContainer.HasTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Hearing")));
}

void ABasicEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO Handle locking of sight
	
	// Sight is Active
	if (IsSightEnabled())
	{
		// TODO Depends on the crouching and distance
		if (GameplayTagsContainer.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone"))))
		{
			SightBar->AddAmount(SightIncreaseRate * DeltaTime);
		}
		else
		{
			SightBar->RemoveAmount(SightDecreaseRate * DeltaTime);
		}
	}

	// Hearing is Active
	if (IsHearingEnabled())
	{
		if (HearingBar->CurrentValue > 0)
		{
			HearingBar->RemoveAmount(HearingDecreaseRate * DeltaTime);
		}
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
		if (!IsSightEnabled()) break;

		if (Actor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				GameplayTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
				PlayerEnteredInSightCone();
				OnPlayerEnteredInSightConeDelegate.Broadcast();
			}
			else
			{
				GameplayTagsContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Character.Sensing.Sight.PlayerIsInCone")));
				PlayerExitedFromSightCone();
				OnPlayerExitedFromSightConeDelegate.Broadcast();
			}
		}
		break;
	case 1:
		if (!IsHearingEnabled()) break;

		if (Stimulus.WasSuccessfullySensed())
		{
			//TODO It depends on the Stimulus
			CurrentHeardStimulus = Stimulus;
			HearingBar->AddAmount(1.0f);
		}
		
		break;
	default:
		break;
	}
}

void ABasicEnemyController::SightBarFull()
{
	GameplayTagsContainer.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Sensing.Sight.PlayerIsSeen")));
	PlayerSeen();
	OnPlayerSeenDelegate.Broadcast();
}

void ABasicEnemyController::HearingBarFull()
{
	SomethingWasHeard(CurrentHeardStimulus);
	OnSomethingWasHeardDelegate.Broadcast(CurrentHeardStimulus);
}

void ABasicEnemyController::BeginPlay()
{
	Super::BeginPlay();

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicEnemyController::ReceiveStimulus);
	EnableSightSense(true);
	EnableHearingSense(true);
}

void ABasicEnemyController::OnPossess(APawn* PossessedPawn)
{
	Super::OnPossess(PossessedPawn);
}
