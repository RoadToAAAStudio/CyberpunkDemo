// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Utility/SplineContainer.h"
#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

class UAIWeaponConfigData;
class UBasicEnemyConfigData;
class UStateTree;
class AAIZone;
class USettableStateTreeComponent;
class ABasicEnemyController;

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY() ABasicEnemyController* BasicEnemyController;
	UPROPERTY(EditDefaultsOnly) AAIZone* AIZone;
#pragma region INPUT_DATA
#pragma region DEFINE_ARCHETYPE
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")	TObjectPtr<UBasicEnemyConfigData> ConfigData;
#pragma endregion
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "AI") TObjectPtr<UAIWeaponConfigData> WeaponConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")	TObjectPtr<ASplineContainer> PatrolSpline;
#pragma endregion

public:
	ABasicEnemy();
	UFUNCTION(BlueprintCallable) void RegisterAIZone(AAIZone* NewAIZone);
	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

private:
	UFUNCTION()	void NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};