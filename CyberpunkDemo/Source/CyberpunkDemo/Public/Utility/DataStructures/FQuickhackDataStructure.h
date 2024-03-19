#pragma once

#include "FQuickhackDataStructure.generated.h"

USTRUCT(BlueprintType)
struct FQuickhackDataStructure : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	UTexture2D* HackImage;

	UPROPERTY(EditAnywhere)
	FString HackName;

	UPROPERTY(EditAnywhere)
	FString HackCost;
};
