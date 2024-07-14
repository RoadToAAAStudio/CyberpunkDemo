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
enum class EBasicEnemyBehaviour : uint8;

UCLASS()
class CYBERPUNKDEMO_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI | Configuration")
	TObjectPtr<UBasicEnemyConfigData> ConfigData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "AI | Configuration")
	TObjectPtr<UAIWeaponConfigData> WeaponConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<ABasicEnemyController> BasicEnemyController;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AAIZone> AIZone;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
	TObjectPtr<ASplineContainer> PatrolSpline;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI | Debug")
	EBasicEnemyBehaviour BehaviourToDebug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Debug")
	bool bDebugKnowledge = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Debug")
	bool bDebugBehaviours = false;

	ABasicEnemy();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable) 
	void RegisterAIZone(AAIZone* NewAIZone);

protected:
	virtual void BeginPlay() override;

private:
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	void DebugKnowledge() const;
	void DebugBehaviours() const;
#endif

	UFUNCTION()	
	void NotifySomethingEnteredInTheTrigger(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};