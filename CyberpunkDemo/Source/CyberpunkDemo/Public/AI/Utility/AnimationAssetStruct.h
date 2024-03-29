#pragma once

#include "AnimationAssetStruct.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FAnimationAssetStruct
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimationAsset> Animation;
};
