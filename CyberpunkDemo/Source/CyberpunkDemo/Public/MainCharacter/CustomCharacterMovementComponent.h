// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/StateMachine.h"
#include "DrawDebugHelpers.h"
#include "CustomCharacterMovementComponent.generated.h"

class AMainCharacter;


UENUM(BlueprintType)
enum class ECustomMovementState : uint8
{
	Idle,
	Walking,
	Running,
	Crouching,
	Jumping,
	Sliding,
	Max UMETA(Hidden)
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UStateMachine> StateMachine;

	
	// Sets default values for this component's properties
	UCustomCharacterMovementComponent();

	UPROPERTY()
	TObjectPtr<AMainCharacter> MainCharacter;

	UPROPERTY(EditDefaultsOnly, Category="Movement|Walk") float Walk_MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Movement|Sprint") float Sprint_MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Movement|Crouch") float Crouch_MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Crouch") float Crouch_HalfHeight;

	// MANTLE PROPERTIES
	
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleMaxDistance = 200;
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleReachHeight = 70;
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MinMantleDepth = 30;
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleMinWallSteepnessAngle = 75;
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleMaxSurfaceAngle = 40;
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleMaxAlignmentAngle = 45;

	
	// Bools used to handle movement state transitions
	UPROPERTY(BlueprintReadOnly) bool bWantsToRun;
	UPROPERTY(BlueprintReadOnly) bool bWantsToCrouchCustom;
	UPROPERTY(BlueprintReadOnly) bool bWantsToJump;

	
private:

	ECustomMovementState CurrentMovementState = ECustomMovementState::Idle;
	
public:
	
	// State machine transitions methods
    #pragma region
	// From IDLE state
	bool CanWalkFromIdle();
	bool CanRunFromIdle();
	bool CanCrouchFromIdle();
	bool CanJumpFromIdle();

	// From WALKING state
	bool CanIdleFromWalk();
	bool CanRunFromWalk();
	bool CanCrouchFromWalk();
	bool CanJumpFromWalk();

	// From RUNNING state
	bool CanIdleFromRun();
	bool CanWalkFromRun();
	bool CanJumpFromRun();

	// From JUMP state
	bool CanIdleFromJump();
	bool CanWalkFromJump();
	bool CanRunFromJump();
	bool CanCrouchFromJump();

	//From CROUCHING state
	bool CanIdleFromCrouch();
	bool CanWalkFromCrouch();
	bool CanRunFromCrouch();
	bool CanJumpFromCrouch();
#pragma endregion 
	
	// Sprint methods
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	// Jump methods
	UFUNCTION(BlueprintCallable) void JumpPressed();
	UFUNCTION(BlueprintCallable) void JumpReleased();

	// Crouch methods
	UFUNCTION(Blueprintable) void CrouchPressed();

	// To get the current state and set
	UFUNCTION(BlueprintCallable) ECustomMovementState GetCurrentMovementState() const;

	void SetCurrentMovementState(ECustomMovementState NewState);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	float GetCapsuleRadius() const;

	float GetCapsuleHalfHeight() const;

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	void BuildStateMachine();

	// Mantle methods

	bool TryMantle();
};
