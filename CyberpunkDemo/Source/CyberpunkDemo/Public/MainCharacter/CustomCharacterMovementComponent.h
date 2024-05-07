// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/StateMachine.h"
#include "DrawDebugHelpers.h"
#include "CustomCharacterMovementComponent.generated.h"

class AMainCharacter;
class UStateCyberpunkProject;

UENUM(BlueprintType)
enum class ECustomMovementState : uint8
{
	Idle,
	Walking,
	Running,
	Crouching,
	Jumping,
	Mantling,
	Vaulting,
	Dashing,
	Max UMETA(Hidden)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnterCustomMovementStateSignature, ECustomMovementState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExitCustomMovementStateSignature, ECustomMovementState, NewState);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CYBERPUNKDEMO_API UCustomCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

// FRIEND CLASSES
friend AMainCharacter;
friend UStateCyberpunkProject;
	
public:

	// Sets default values for this component's properties
	UCustomCharacterMovementComponent();

	UPROPERTY() TObjectPtr<AMainCharacter> MainCharacter;

	// DELEGATES
	FOnEnterCustomMovementStateSignature OnEnterCustomMovementState;
	FOnExitCustomMovementStateSignature OnExitCustomMovementState;

#pragma region MOVEMENT_PROPERTIES
	// WALK PROPERTIES
	UPROPERTY(EditDefaultsOnly, Category="Movement|Walk") float Walk_MaxWalkSpeed = 300.0f;;

	// RUN PROPERTIES
	UPROPERTY(EditDefaultsOnly, Category="Movement|Sprint") float Sprint_MaxWalkSpeed = 600.0f;;

	// CROUCH PROPERTIES
	UPROPERTY(EditDefaultsOnly, Category="Movement|Crouch") float Crouch_MaxWalkSpeed = 150.0f;;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Crouch", meta = (ClampMin = 55.f)) float Crouch_HalfHeight = 55.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Crouch") float Crouch_BlendSpeed = 0.3f;

	// JUMP PROPERTIES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Jump") float JumpForce = 500.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Jump") float SecondJumpForce = 900.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Jump") bool bCanDoubleJump = true;

	// GRAVITY PROPERTIES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Gravity") float CustomGravity = 2.0f;

	// DASH PROPERTIES
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Dash") float DashDuration = 0.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Movement|Dash") float DashSpeedMultiplier = 10.0f;
	
	// MANTLE PROPERTIES
	// Max distance to check for a possible mantle
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleMaxDistance = 10.0f;
	// Max height to check for a possible mantle
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleReachHeight = 70.0f;
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MinMantleDepth = 30.0f;
	// Offset used to ignore obstacles under a certain height
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleBaseStartOffset = 40.0f;
	// Height used to decide if a mantle should be a "high" or "low" one
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float LowMantleCutoff = 120.0f;
	// Minimum steepness accepted for the wall the character should mantle on (expressed in degrees)
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleMinWallSteepnessAngle = 75.0f;
	// Max steepness accepted for the surface the character should mantle to (expressed in degrees)
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleMaxSurfaceAngle = 40.0f;
	UPROPERTY(EditDefaultsOnly, Category="Movement|Mantle") float MantleMaxAlignmentAngle = 45.0f;
	UPROPERTY(BlueprintReadOnly) FVector MantleLocation;
	
	// VAULT PROPERTIES
	// Max distance to check for a possible vault
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Vault") float VaultMaxDistanceCheck = 100.0f;
	// Max height to check for a possible vault
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Vault") float VaultMaxPossibleHeight = 100.0f;
	// Max width of a possible vaultable object 
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Vault") float VaultMaxPossibleWidth = 25.0f;
	// 
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Vault") float VaultLandingPointMaxHeight;
	// Offset used to ignore obstacles under a certain height
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Vault") float VaultBaseStartOffset = 20.0f;
	UPROPERTY(BlueprintReadOnly) FVector VaultLocation;
	UPROPERTY(BlueprintReadOnly) FVector VaultMiddleLocation;
	UPROPERTY(BlueprintReadOnly) float VaultLerpSpeed;
#pragma endregion
	
	// Bools used to handle movement state transitions
	UPROPERTY(BlueprintReadOnly) bool bWantsToRun = false;
	UPROPERTY(BlueprintReadOnly) bool bWantsToCrouchCustom = false;
	UPROPERTY(BlueprintReadOnly) bool bWantsToJump = false;
	UPROPERTY(BlueprintReadOnly) bool bWantsToDash = false;
	UPROPERTY(BlueprintReadWrite) bool bCanMantle = false;
	UPROPERTY(BlueprintReadWrite) bool bCanVault = false;

	// Bools used to determine whether the kind of mantle/vault to perform
	UPROPERTY(BlueprintReadOnly) bool bHighMantle = false;
	UPROPERTY(BlueprintReadOnly) bool bFallingVault = false;
	
	// Field public for debug reason
	UPROPERTY(BlueprintReadOnly) TObjectPtr<UStateMachine> StateMachine;

private:

	ECustomMovementState CurrentMovementState = ECustomMovementState::Idle;
	ECustomMovementState LastMovementState = ECustomMovementState::Idle;

public:

	// To get the current state and set
	UFUNCTION(BlueprintCallable) ECustomMovementState GetCurrentMovementState() const;
	UFUNCTION(BlueprintCallable) ECustomMovementState GetLastMovementState() const;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	float GetCapsuleRadius() const;

	float GetCapsuleHalfHeight() const;
	
protected:
	
	virtual void BeginPlay() override;

private:

	void BuildStateMachine();

	void SetCurrentMovementState(ECustomMovementState NewState);
	void SetLastMovementState(ECustomMovementState NewState);

#pragma region STATE_MACHINE_TRANSITIONS_METHODS

	// Used to check whether we can transition from a state to another 
	
	// From IDLE state
	bool CanWalkFromIdle() const;
	bool CanRunFromIdle() const;
	bool CanCrouchFromIdle() const;
	bool CanJumpFromIdle();

	// From WALKING state
	bool CanIdleFromWalk() const;
	bool CanRunFromWalk() const;
	bool CanCrouchFromWalk() const;
	bool CanJumpFromWalk();

	// From RUNNING state
	bool CanIdleFromRun() const;
	bool CanWalkFromRun() const;
	bool CanJumpFromRun();

	// From JUMP state
	bool CanIdleFromJump() const;
	bool CanWalkFromJump() const;
	bool CanRunFromJump() const;
	bool CanCrouchFromJump() const;
	bool CanJumpFromJump() const;

	//From CROUCHING state
	bool CanIdleFromCrouch();
	bool CanWalkFromCrouch();
	bool CanRunFromCrouch();
	bool CanJumpFromCrouch();

	// From MANTLE state
	bool CanIdleFromMantle() const;

	// From VAULT state
	bool CanIdleFromVault() const;

	// From DASH state
	bool CanIdleFromDash() const;

	// From ANY
	bool CanMantleFromAny() const;
	bool CanVaultFromAny() const;
	bool CanDashFromAny() const;

	bool TryMantle();
	bool TryVault();
	bool CanUncrouch();
	
#pragma endregion 

#pragma region INPUT_METHODS
	// Sprint methods
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	// Jump methods
	UFUNCTION(BlueprintCallable) void JumpPressed();

	// Crouch methods
	UFUNCTION(BlueprintCallable) void CrouchPressed();

	// Dash methods
	UFUNCTION(BlueprintCallable) void DashPressed();
#pragma endregion 
};
