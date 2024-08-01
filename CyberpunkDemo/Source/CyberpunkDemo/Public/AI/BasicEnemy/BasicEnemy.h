// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

class UAIWeaponConfigData;
class UBasicEnemyConfigData;
class AAIZone;
class ABasicEnemyController;
class ASplineContainer;

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI | Configuration")
	TObjectPtr<UBasicEnemyConfigData> ConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "AI | Configuration")
	TObjectPtr<UAIWeaponConfigData> WeaponConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<ABasicEnemyController> BasicEnemyController;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<AAIZone> AIZone;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<ASplineContainer> PatrolSpline;

	ABasicEnemy();

	void RegisterAIZone(AAIZone* NewAIZone);

protected:
	virtual void BeginPlay() override;
};