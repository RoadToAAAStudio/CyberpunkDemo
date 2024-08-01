// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "AI/Utility/IClaimable.h"
#include "GameFramework/Actor.h"
#include "Location.generated.h"

class USphereComponent;
class AAIZone;

UCLASS()
class CYBERPUNKDEMO_API ALocation : public AActor, public IGameplayTagAssetInterface, public IClaimable
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ALocation();

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> SphereTrigger;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI | Configuration")
	TObjectPtr<AAIZone> AIZone;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "AI | Knowledge")
	FGameplayTagContainer GameplayTagContainer;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "AI | Knowledge")
	TObjectPtr<AActor> ClaimOwner;

#pragma region INTERFACE_METHODS
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	bool Claim_Implementation(AActor* NewOwner) override;
	bool Release_Implementation(const AActor* OldOwner) override;
	bool IsUsed_Implementation() const override;
	bool IsUsedBy_Implementation(const AActor* PotentialOwner) const override;
	bool IsClaimable_Implementation(const AActor* PotentialOwner) const override;
#pragma endregion
};


