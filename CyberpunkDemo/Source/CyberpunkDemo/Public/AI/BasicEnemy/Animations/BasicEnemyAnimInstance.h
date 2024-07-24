// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BasicEnemyAnimInstance.generated.h"

class ABasicEnemy;
class UCharacterMovementComponent;

UCLASS()
class CYBERPUNKDEMO_API UBasicEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ABasicEnemy> BasicEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GroundSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsCrouching = false;
};
