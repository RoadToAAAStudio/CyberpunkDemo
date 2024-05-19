// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Knowledge/BasicEnemyPerceptionComponent.h"
#include "AI/BasicEnemy/BasicEnemyConfigData.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

void UBasicEnemyPerceptionComponent::SetUpFromData(const UBasicEnemyPerceptionData* PerceptionConfigData)
{
	// const FKnowledgeConfigData* ConfigData = nullptr;
	// for (auto& RowName : KnowledgeConfigData->GetRowNames())
	// {
	// 	ConfigData = KnowledgeConfigData->FindRow<FKnowledgeConfigData>(RowName, "");
	// 	if (!ConfigData) continue;
	// 	break;
	// }
	
	for (auto& SenseConfig : SensesConfig)
	{
		if (SenseConfig->GetClass() == UAISenseConfig_Sight::StaticClass())
		{
			UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(SenseConfig);
			SightConfig->SightRadius		            = PerceptionConfigData->SightRadius;
			SightConfig->LoseSightRadius	            = PerceptionConfigData->SightLoseRadius;
			SightConfig->PeripheralVisionAngleDegrees	= PerceptionConfigData->SightPeripheralVisionAngleDegrees;
			SightConfig->NearClippingRadius				= PerceptionConfigData->SightNearClippingRadius;	
			SightConfig->PointOfViewBackwardOffset		= PerceptionConfigData->SightPointOfViewBackwardOffset;
			ConfigureSense(*SightConfig);
		}
		else if (SenseConfig->GetClass() == UAISenseConfig_Hearing::StaticClass())
		{
			UAISenseConfig_Hearing* HearingConfig = Cast<UAISenseConfig_Hearing>(SenseConfig);
			HearingConfig->HearingRange	= PerceptionConfigData->HearingRange;
			ConfigureSense(*HearingConfig);
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