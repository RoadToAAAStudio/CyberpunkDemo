// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "AI/Utility/IClaimable.h"
#include "GameFramework/Actor.h"
#include "Location.generated.h"

class USphereComponent;

UCLASS()
class CYBERPUNKDEMO_API ALocation : public AActor, public IGameplayTagAssetInterface, public IClaimable
{
private:
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ALocation();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereTrigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> ClaimOwner;

public:	
#pragma region INTERFACE_METHODS
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	bool Claim_Implementation(AActor* NewOwner) override;
	bool Release_Implementation(const AActor* OldOwner) override;
	bool IsUsed_Implementation() const override;
	bool IsUsedBy_Implementation(const AActor* PotentialOwner) const override;
	bool IsClaimable_Implementation(const AActor* PotentialOwner) const override;
#pragma endregion
};


