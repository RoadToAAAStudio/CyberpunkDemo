// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Knowledge/BasicEnemyPerceptionComponent.h"
#include "AI/BasicEnemy/BasicEnemyConfigData.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

void UBasicEnemyPerceptionComponent::SetUpFromData(const UDataTable* KnowledgeConfigData)
{
	const FKnowledgeConfigData* ConfigData = nullptr;
	for (auto& RowName : KnowledgeConfigData->GetRowNames())
	{
		ConfigData = KnowledgeConfigData->FindRow<FKnowledgeConfigData>(RowName, "");
		if (!ConfigData) continue;
		break;
	}
	
	for (auto& SenseConfig : SensesConfig)
	{
		if (SenseConfig->GetClass() == UAISenseConfig_Sight::StaticClass())
		{
			UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(SenseConfig);
			SightConfig->SightRadius		            = ConfigData->SightRadius;
			SightConfig->LoseSightRadius	            = ConfigData->SightLoseRadius;
			SightConfig->PeripheralVisionAngleDegrees	= ConfigData->SightPeripheralVisionAngleDegrees;
			SightConfig->NearClippingRadius				= ConfigData->SightNearClippingRadius;	
			SightConfig->PointOfViewBackwardOffset		= ConfigData->SightPointOfViewBackwardOffset;
		}
		else if (SenseConfig->GetClass() == UAISenseConfig_Hearing::StaticClass())
		{
			UAISenseConfig_Hearing* HearingConfig = Cast<UAISenseConfig_Hearing>(SenseConfig);
			HearingConfig->HearingRange					= ConfigData->HearingRange;
		}
	}
}

TArray<UAISenseConfig*>& UBasicEnemyPerceptionComponent::GetSenses() { return SensesConfig; }

UAISenseConfig* UBasicEnemyPerceptionComponent::GetSense(TSubclassOf<UAISenseConfig> Type)
{
	for (const auto& SenseConfig : GetSenses())
	{
		if (SenseConfig->GetClass() == Type)
		{
			return SenseConfig;
		}
	}

	return nullptr;
}
