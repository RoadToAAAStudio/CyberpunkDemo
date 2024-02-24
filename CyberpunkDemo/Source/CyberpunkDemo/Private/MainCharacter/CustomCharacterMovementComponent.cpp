// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/CustomCharacterMovementComponent.h"

#include "Utility/States/StateIdle.h"
#include "Utility/FTransition.h"
#include "Utility/StateMachine.h"
#include "Utility/States/StateCrouching.h"
#include "Utility/States/StateJumping.h"
#include "Utility/States/StateRunning.h"
#include "Utility/States/StateWalking.h"


// Sets default values for this component's properties
UCustomCharacterMovementComponent::UCustomCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsSprinting = false;
	bIsCrouching = false;
	Walk_MaxWalkSpeed = 500.0f;
	Sprint_MaxWalkSpeed = 1000.0f;
}


// Called when the game starts
void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//BuildStateMachine();
}

void UCustomCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (bIsSprinting)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}
// STATE MACHINE
#pragma region 
// Called at begin play to create and setup the movement state machine
void UCustomCharacterMovementComponent::BuildStateMachine()
{
	// Create the state machine object
	TObjectPtr<UStateMachine> StateMachine = NewObject<UStateMachine>();

	// Create the states and add them to the state machine
	TObjectPtr<UStateIdle> StateIdle = NewObject<UStateIdle>();
	StateMachine->AddState(StateIdle);
	
	TObjectPtr<UStateWalking> StateWalking = NewObject<UStateWalking>();
	StateMachine->AddState(StateWalking);
	
	TObjectPtr<UStateRunning> StateRunning = NewObject<UStateRunning>();
	StateMachine->AddState(StateRunning);

	TObjectPtr<UStateJumping> StateJumping = NewObject<UStateJumping>();
	StateMachine->AddState(StateJumping);

	TObjectPtr<UStateCrouching> StateCrouching = NewObject<UStateCrouching>();
	StateMachine->AddState(StateCrouching);

	// Create the transitions and add them to the state machine
	// Idle TO Walking
	TObjectPtr<UFTransition> IdleToWalking = NewObject<UFTransition>();
	StateMachine->AddTransition(StateIdle, IdleToWalking);
	TFunction<bool()> IdleToWalkingCondition = &UCustomCharacterMovementComponent::CanWalkFromIdle;
	IdleToWalking->Init(StateWalking, IdleToWalkingCondition);

	// Idle TO Jumping
	TObjectPtr<UFTransition> IdleToJumping = NewObject<UFTransition>();
	StateMachine->AddTransition(StateIdle, IdleToJumping);
	TFunction<bool()> IdleToJumpingCondition = &UCustomCharacterMovementComponent::CanJumpFromIdle;
	IdleToJumping->Init(StateJumping, IdleToJumpingCondition);

	// Idle TO Crouching
	TObjectPtr<UFTransition> IdleToCrouching = NewObject<UFTransition>();
	StateMachine->AddTransition(StateIdle, IdleToCrouching);
	TFunction<bool()> IdleToCrouchCondition = &UCustomCharacterMovementComponent::CanCrouchFromIdle;
	IdleToCrouching->Init(StateCrouching, IdleToCrouchCondition);

	// Idle TO Running
	TObjectPtr<UFTransition> IdleToRunning = NewObject<UFTransition>();
	StateMachine->AddTransition(StateRunning, IdleToRunning);
	TFunction<bool()> IdleToRunningCondition = &UCustomCharacterMovementComponent::CanRunFromIdle;
	IdleToRunning->Init(StateRunning, IdleToRunningCondition);
}

// STATE MACHINE CONDITION CHECKER METHODS

bool UCustomCharacterMovementComponent::CanWalkFromIdle()
{
	return true;
}

bool UCustomCharacterMovementComponent::CanRunFromIdle()
{
	return true;
}

bool UCustomCharacterMovementComponent::CanCrouchFromIdle()
{
	return true;
}

bool UCustomCharacterMovementComponent::CanJumpFromIdle()
{
	return true;
}

#pragma endregion 

// Called every frame
void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UCustomCharacterMovementComponent::SprintPressed()
{
	bIsSprinting = true;
}

void UCustomCharacterMovementComponent::SprintReleased()
{
	bIsSprinting = false;
}


