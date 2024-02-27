// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/CustomCharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "MainCharacter/MainCharacter.h"
#include "Utility/States/StateIdle.h"
#include "Utility/FTransition.h"
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

	bWantsToJump = false;
	bWantsToRun = false;
	bWantsToCrouchCustom = false;
	Walk_MaxWalkSpeed = 500.0f;
	Sprint_MaxWalkSpeed = 1000.0f;
}


// Called when the game starts
void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	BuildStateMachine();
}

void UCustomCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

}


// STATE MACHINE
#pragma region 
// Called at begin play to create and setup the movement state machine
void UCustomCharacterMovementComponent::BuildStateMachine()
{
	// Create the state machine object
	StateMachine = NewObject<UStateMachine>();

	// Create the states and add them to the state machine
	TObjectPtr<UStateIdle> StateIdle = NewObject<UStateIdle>();
	StateMachine->AddState(StateIdle);
	StateIdle->SetOwner(this);
	
	TObjectPtr<UStateWalking> StateWalking = NewObject<UStateWalking>();
	StateMachine->AddState(StateWalking);
	StateWalking->SetOwner(this);
	
	TObjectPtr<UStateRunning> StateRunning = NewObject<UStateRunning>();
	StateMachine->AddState(StateRunning);
	StateRunning->SetOwner(this);

	TObjectPtr<UStateJumping> StateJumping = NewObject<UStateJumping>();
	StateMachine->AddState(StateJumping);
	StateJumping->SetOwner(this);

	TObjectPtr<UStateCrouching> StateCrouching = NewObject<UStateCrouching>();
	StateMachine->AddState(StateCrouching);
	StateCrouching->SetOwner(this);

	StateMachine->Init(StateIdle);

	// Create the transitions and add them to the state machine
	// IDLE
	// Idle TO Walking
	TObjectPtr<UFTransition> IdleToWalking = NewObject<UFTransition>();
	StateIdle->Transitions.Add(IdleToWalking);
	IdleToWalking->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanWalkFromIdle);
	IdleToWalking->Init(StateWalking);

	// Idle TO Jumping
	TObjectPtr<UFTransition> IdleToJumping = NewObject<UFTransition>();
	StateIdle->Transitions.Add(IdleToJumping);
	IdleToJumping->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromIdle);
	IdleToJumping->Init(StateJumping);

	// Idle TO Crouching
	TObjectPtr<UFTransition> IdleToCrouching = NewObject<UFTransition>();
	StateIdle->Transitions.Add(IdleToCrouching);
	IdleToCrouching->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanCrouchFromIdle);
	IdleToCrouching->Init(StateCrouching);

	// Idle TO Running
	TObjectPtr<UFTransition> IdleToRunning = NewObject<UFTransition>();
	StateIdle->Transitions.Add(IdleToRunning);
	IdleToRunning->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanRunFromIdle);
	IdleToRunning->Init(StateRunning);

	// WALKING
	// Walking TO Idle
	TObjectPtr<UFTransition> WalkingToIdle = NewObject<UFTransition>();
	StateWalking->Transitions.Add(WalkingToIdle);
	WalkingToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromWalk);
	WalkingToIdle->Init(StateIdle);
	
	// Walking TO Running
	TObjectPtr<UFTransition> WalkingToRunning = NewObject<UFTransition>();
	StateWalking->Transitions.Add(WalkingToRunning);
	WalkingToRunning->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanRunFromWalk);
	WalkingToRunning->Init(StateRunning);

	// Walking TO Crouching
	TObjectPtr<UFTransition> WalkingToCrouching = NewObject<UFTransition>();
	StateWalking->Transitions.Add(WalkingToCrouching);
	WalkingToCrouching->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanCrouchFromWalk);
	WalkingToCrouching->Init(StateCrouching);

	// Walking TO Jump
	TObjectPtr<UFTransition> WalkingToJump = NewObject<UFTransition>();
	StateWalking->Transitions.Add(WalkingToJump);
	WalkingToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromWalk);
	WalkingToJump->Init(StateJumping);

	// RUNNING
	// Running TO Idle
	TObjectPtr<UFTransition> RunningToIdle = NewObject<UFTransition>();
	StateRunning->Transitions.Add(RunningToIdle);
	RunningToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromRun);
	RunningToIdle->Init(StateIdle);

	// Running TO Walking
	TObjectPtr<UFTransition> RunningToWalking = NewObject<UFTransition>();
	StateRunning->Transitions.Add(RunningToWalking);
	RunningToWalking->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanWalkFromRun);
	RunningToWalking->Init(StateWalking);

	// Running TO Jump
	TObjectPtr<UFTransition> RunningToJump = NewObject<UFTransition>();
	StateRunning->Transitions.Add(RunningToJump);
	RunningToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromRun);
	RunningToJump->Init(StateJumping);

	// JUMP
	// Jump TO Idle
	TObjectPtr<UFTransition> JumpToIdle = NewObject<UFTransition>();
	StateJumping->Transitions.Add(JumpToIdle);
	JumpToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromJump);
	JumpToIdle->Init(StateIdle);

	// Jump TO Walking
	TObjectPtr<UFTransition> JumpToWalking = NewObject<UFTransition>();
	StateJumping->Transitions.Add(JumpToWalking);
	JumpToWalking->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanWalkFromJump);
	JumpToWalking->Init(StateWalking);

	// Jump TO Running
	TObjectPtr<UFTransition> JumpToRunning = NewObject<UFTransition>();
	StateJumping->Transitions.Add(JumpToRunning);
	JumpToRunning->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanRunFromJump);
	JumpToRunning->Init(StateJumping);

	// Jump TO Crouch
	TObjectPtr<UFTransition> JumpToCrouch = NewObject<UFTransition>();
	StateJumping->Transitions.Add(JumpToCrouch);
	JumpToCrouch->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanCrouchFromJump);
	JumpToCrouch->Init(StateCrouching);
	
	// CROUCH
	// Crouch TO Idle
	TObjectPtr<UFTransition> CrouchToIdle = NewObject<UFTransition>();
	StateCrouching->Transitions.Add(CrouchToIdle);
	CrouchToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromCrouch);
	CrouchToIdle->Init(StateIdle);

	// Crouch TO Walking
	TObjectPtr<UFTransition> CrouchToWalking = NewObject<UFTransition>();
	StateCrouching->Transitions.Add(CrouchToWalking);
	CrouchToWalking->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanWalkFromCrouch);
	CrouchToWalking->Init(StateWalking);

	// Crouch TO Running
	TObjectPtr<UFTransition> CrouchToRunning = NewObject<UFTransition>();
	StateCrouching->Transitions.Add(CrouchToRunning);
	CrouchToRunning->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanRunFromCrouch);
	CrouchToRunning->Init(StateRunning);

	// Crouch TO Jump
	TObjectPtr<UFTransition> CrouchToJump = NewObject<UFTransition>();
	StateCrouching->Transitions.Add(CrouchToJump);
	CrouchToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromCrouch);
	CrouchToJump->Init(StateJumping);
}

void UCustomCharacterMovementComponent::SetCurrentMovementState(ECustomMovementState NewState)
{
	CurrentMovementState = NewState;
}

// STATE MACHINE CONDITION CHECKER METHODS
// FROM IDLE
bool UCustomCharacterMovementComponent::CanWalkFromIdle()
{
	return IsMovingOnGround() && !Velocity.IsZero();
}

bool UCustomCharacterMovementComponent::CanRunFromIdle()
{
	return bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanCrouchFromIdle()
{
	return bWantsToCrouchCustom;
}

bool UCustomCharacterMovementComponent::CanJumpFromIdle()
{
	return bWantsToJump;
}

// FROM WALKING
bool UCustomCharacterMovementComponent::CanIdleFromWalk()
{
	return IsMovingOnGround() && Velocity.IsZero();
}

bool UCustomCharacterMovementComponent::CanRunFromWalk()
{
	return IsMovingOnGround() && bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanCrouchFromWalk()
{
	return IsMovingOnGround() && bWantsToCrouchCustom;
}

bool UCustomCharacterMovementComponent::CanJumpFromWalk()
{
	return IsMovingOnGround() && bWantsToJump;
}

// FROM RUNNING
bool UCustomCharacterMovementComponent::CanIdleFromRun()
{
	return IsMovingOnGround() && Velocity.IsZero() && !bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanWalkFromRun()
{
	return IsMovingOnGround() && !bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanJumpFromRun()
{
	return IsMovingOnGround() && bWantsToJump;
}

// FROM JUMP
bool UCustomCharacterMovementComponent::CanIdleFromJump()
{
	return IsMovingOnGround() && Velocity.IsZero();
}

bool UCustomCharacterMovementComponent::CanWalkFromJump()
{
	return IsMovingOnGround() && !Velocity.IsZero();
}

bool UCustomCharacterMovementComponent::CanRunFromJump()
{
	return IsMovingOnGround() && !Velocity.IsZero() && bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanCrouchFromJump()
{
	return IsMovingOnGround() && bWantsToCrouchCustom && !bWantsToJump;
}

// FROM CROUCH
bool UCustomCharacterMovementComponent::CanIdleFromCrouch()
{
	return IsMovingOnGround() && Velocity.IsZero() && !bWantsToCrouchCustom;
}

bool UCustomCharacterMovementComponent::CanWalkFromCrouch()
{
	return IsMovingOnGround() && !Velocity.IsZero() && !bWantsToCrouchCustom;
}

bool UCustomCharacterMovementComponent::CanRunFromCrouch()
{
	return IsMovingOnGround() && !Velocity.IsZero() && bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanJumpFromCrouch()
{
	return IsMovingOnGround() && bWantsToJump;
}

#pragma endregion 

// Called every frame
void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	StateMachine->Tick();
}

void UCustomCharacterMovementComponent::SprintPressed()
{
	bWantsToRun = true;
}

void UCustomCharacterMovementComponent::SprintReleased()
{
	bWantsToRun = false;
}

void UCustomCharacterMovementComponent::JumpPressed()
{
	bWantsToJump = true;
}

void UCustomCharacterMovementComponent::JumpReleased()
{
	bWantsToJump = false;
}

void UCustomCharacterMovementComponent::CrouchPressed()
{
	bWantsToCrouchCustom = !bWantsToCrouchCustom;
}

ECustomMovementState UCustomCharacterMovementComponent::GetCurrentMovementState() const
{
	return CurrentMovementState;
}


