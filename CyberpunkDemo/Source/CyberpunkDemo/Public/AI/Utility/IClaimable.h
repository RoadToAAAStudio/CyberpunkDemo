// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IClaimable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UClaimable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CYBERPUNKDEMO_API IClaimable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool Claim(AActor* NewOwner);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool Release(const AActor* OldOwner);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool IsUsed() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool IsUsedBy(const AActor* PotentialOwner) const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) bool IsClaimable(const AActor* PotentialOwner) const;
};
