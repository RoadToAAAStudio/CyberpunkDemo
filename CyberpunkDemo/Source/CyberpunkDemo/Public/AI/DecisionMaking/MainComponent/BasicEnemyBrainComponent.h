// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicEnemyBrainComponent.generated.h"


class UBehaviour;
class UBasicEnemyKnowledgeComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UBasicEnemyBrainComponent : public UActorComponent
{
	GENERATED_BODY()
private:
#pragma region INJECTED_DEPENDENCIES
	UPROPERTY() const UBasicEnemyKnowledgeComponent* KnowledgeComponent;
#pragma endregion
	UPROPERTY() TArray<TObjectPtr<UBehaviour>> SupportedBehaviours;
	
public:
	UBasicEnemyBrainComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetUpFromData(const UDataTable* ConfigData);
	void Initialize(const UBasicEnemyKnowledgeComponent* KnowledgeComponentInput);
	
};
