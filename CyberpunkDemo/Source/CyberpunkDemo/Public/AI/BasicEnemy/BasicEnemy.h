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

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI | Configuration")
	TObjectPtr<AAIZone> AIZone;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI | Configuration")
	TObjectPtr<ASplineContainer> PatrolSpline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<ABasicEnemyController> BasicEnemyController;

	ABasicEnemy();

protected:
	virtual void BeginPlay() override;
};