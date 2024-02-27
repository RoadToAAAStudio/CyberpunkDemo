// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/StateMachine.h"
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

	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;

	// Bools used to handle movement state transitions
	bool bWantsToRun;
	bool bWantsToCrouch;
	bool bWantsToJump;

	
private:

	ECustomMovementState CurrentMovementState = ECustomMovementState::Idle;
	
public:
	
	// State machine transitions methods
	bool CanWalkFromIdle();
	bool CanRunFromIdle();
	bool CanCrouchFromIdle();
	bool CanJumpFromIdle();

	bool CanIdleFromWalk();
	bool CanRunFromWalk();
	bool CanCrouchFromWalk();
	bool CanJumpFromWalk();

	bool CanIdleFromRun();
	bool CanWalkFromRun();
	bool CanJumpFromRun();

	// Sprint methods
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();

	// Jump methods
	UFUNCTION(BlueprintCallable) void JumpPressed();
	UFUNCTION(BlueprintCallable) void JumpReleased();

	// To get the current state and set
	ECustomMovementState GetCurrentMovementState() const;

	void SetCurrentMovementState(ECustomMovementState NewState);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	void BuildStateMachine();
};
