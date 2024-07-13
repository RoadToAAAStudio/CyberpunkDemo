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
class USettableStateTreeComponent;
class UStateTree;

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY() TObjectPtr<ABasicEnemyController> BasicEnemyController;
	UPROPERTY(EditDefaultsOnly) TObjectPtr<AAIZone> AIZone;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")	TObjectPtr<UBasicEnemyConfigData> ConfigData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "AI") TObjectPtr<UAIWeaponConfigData> WeaponConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")	TObjectPtr<ASplineContainer> PatrolSpline;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bDebugKnowledge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	bool bDebugBehaviours = false;

	ABasicEnemy();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable) 
	void RegisterAIZone(AAIZone* NewAIZone);

protected:
	virtual void BeginPlay() override;

private:
	void DebugKnowledge() const;
	void DebugBehaviours() const;

	UFUNCTION()	void NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};