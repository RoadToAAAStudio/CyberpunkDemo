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

		float CHH = CustomCharacterMovementComponent->Crouch_HalfHeight;
		float SCHH = MainCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		
		FVector TargetCrouchOffset = FVector(0,0,CHH - SCHH);
		
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CustomCharacterMovementComponent->Crouch_BlendSpeed, 0.0f, 1.0f));
		
		if (CustomCharacterMovementComponent->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.0f, CustomCharacterMovementComponent->Crouch_BlendSpeed);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.0f, CustomCharacterMovementComponent->Crouch_BlendSpeed);
		}
		
		if (CustomCharacterMovementComponent->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}

		// THIS BREAKS THE CURRENT AIM METHOD
		const FVector ArmsOffset = MainCharacter->GetFirstPersonCameraComponent()->GetComponentTransform().InverseTransformPosition(OutVT.POV.Location);
		MainCharacter->GetMesh1P()->SetRelativeLocation(FVector(-15.f, 0.f, -165.f) + ArmsOffset);
	}
}
