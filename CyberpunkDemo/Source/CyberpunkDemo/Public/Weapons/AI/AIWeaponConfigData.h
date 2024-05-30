// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIWeaponConfigData.generated.h"

/**
 * Weapon configs
 */
UCLASS()
class CYBERPUNKDEMO_API UAIWeaponConfigData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon stats")
	float Damage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon stats")
	float HeadMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon stats")
	float ShootingDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon stats")
	float ReloadTime = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon stats")
	float EffectiveRange = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon stats")
	float Accuracy = 1.0f;
};
