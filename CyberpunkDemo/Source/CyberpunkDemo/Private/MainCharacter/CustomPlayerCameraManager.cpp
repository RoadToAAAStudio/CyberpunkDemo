// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/CustomPlayerCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MainCharacter/MainCharacter.h"

ACustomPlayerCameraManager::ACustomPlayerCameraManager()
{
}

void ACustomPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (TObjectPtr<AMainCharacter> MainCharacter = Cast<AMainCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		TObjectPtr<UCustomCharacterMovementComponent> CustomCharacterMovementComponent = MainCharacter->GetCustomCharacterComponent();

		FVector TargetCrouchOffset = FVector(0,0, CustomCharacterMovementComponent->GetCrouchedHalfHeight() - MainCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime/CrouchBlendDuration, 0, 1));

		if (CustomCharacterMovementComponent->GetCurrentMovementState() == ECustomMovementState::Crouching)
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.0f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0, CrouchBlendDuration);
		}

		if (CustomCharacterMovementComponent->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}

		//const FVector ArmsOffset = MainCharacter->GetFirstPersonCameraComponent()->GetComponentTransform().InverseTransformPosition(OutVT.POV.Location);
		//MainCharacter->GetMesh1P()->SetRelativeLocation(FVector(0, 0, -12) + ArmsOffset);
	}
}
