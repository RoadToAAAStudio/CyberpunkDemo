// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/CustomCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter/MainCharacter.h"
#include "Utility/States/StateIdle.h"
#include "Utility/Transition.h"
#include "Utility/States/StateCrouching.h"
#include "Utility/States/StateDash.h"
#include "Utility/States/StateJumping.h"
#include "Utility/States/StateRunning.h"
#include "Utility/States/StateVault.h"
#include "Utility/States/StateWalking.h"
#include "Utility/States/StateMantle.h"

// Shortcut macros
#if 0
float Duration = 5.0f;
#define PRINT_SCREEN(x) GEngine->AddOnScreenDebugMessage(-1, Duration ? Duration : -1.f, FColor::Yellow, x);
#define DRAW_POINT(x, c) DrawDebugPoint(GetWorld(), x, 10, c, !Duration, Duration);
#define DRAW_LINE(x1, x2, c) DrawDebugLine(GetWorld(), x1, x2, c, !Duration, Duration);
#define DRAW_CAPSULE(x, c) DrawDebugCapsule(GetWorld(), x, GetCapsuleHalfHeight(), GetCapsuleRadius(), FQuat::Identity, c, !Duration, Duration);
#else
#define PRINT_SCREEN(x)
#define DRAW_POINT(x, c)
#define DRAW_LINE(x1, x2, c)
#define DRAW_CAPSULE(x, c)
#endif

// CONSTRUCTOR
UCustomCharacterMovementComponent::UCustomCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	NavAgentProps.bCanCrouch = true;
	bCanWalkOffLedgesWhenCrouching = true;
}

// BEGIN PLAY
void UCustomCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SetCrouchedHalfHeight(Crouch_HalfHeight);
	GravityScale = CustomGravity;
	BuildStateMachine();
}

// STATE MACHINE
// Custom state machine that handles all the different movement states
#pragma region STATE_MACHINE
// Called at begin play to create and setup the movement state machine
void UCustomCharacterMovementComponent::BuildStateMachine()
{
	// Create the state machine object
	StateMachine = NewObject<UStateMachine>();

	// Create the states and add them to the state machine
	TObjectPtr<UStateIdle> StateIdle = NewObject<UStateIdle>();
	StateMachine->AddState(StateIdle);
	StateIdle->Initialize(this, ECustomMovementState::Idle);
	
	TObjectPtr<UStateWalking> StateWalking = NewObject<UStateWalking>();
	StateMachine->AddState(StateWalking);
	StateWalking->Initialize(this, ECustomMovementState::Walking);
	
	TObjectPtr<UStateRunning> StateRunning = NewObject<UStateRunning>();
	StateMachine->AddState(StateRunning);
	StateRunning->Initialize(this, ECustomMovementState::Running);

	TObjectPtr<UStateJumping> StateJumping = NewObject<UStateJumping>();
	StateMachine->AddState(StateJumping);
	StateJumping->Initialize(this, ECustomMovementState::Jumping);

	TObjectPtr<UStateCrouching> StateCrouching = NewObject<UStateCrouching>();
	StateMachine->AddState(StateCrouching);
	StateCrouching->Initialize(this, ECustomMovementState::Crouching);

	TObjectPtr<UStateMantle> StateMantle = NewObject<UStateMantle>();
	StateMachine->AddState(StateMantle);
	StateMantle->Initialize(this, ECustomMovementState::Mantling);

	TObjectPtr<UStateVault> StateVault = NewObject<UStateVault>();
	StateMachine->AddState(StateVault);
	StateVault->Initialize(this, ECustomMovementState::Vaulting);

	TObjectPtr<UStateDash> StateDash = NewObject<UStateDash>();
	StateMachine->AddState(StateDash);
	StateDash->Initialize(this, ECustomMovementState::Dashing);

	StateMachine->Init(StateIdle);

	// Create the transitions and add them to the state machine
	// Bind each transition delegate with a function used to check if the transition is true/false
	// Set the state that transition points to
	// IDLE
	// Idle TO Walking
	TObjectPtr<UTransition> IdleToWalking = NewObject<UTransition>();
	StateIdle->Transitions.Add(IdleToWalking);
	IdleToWalking->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanWalkFromIdle);
	IdleToWalking->Init(StateWalking);

	// Idle TO Jumping
	TObjectPtr<UTransition> IdleToJumping = NewObject<UTransition>();
	StateIdle->Transitions.Add(IdleToJumping);
	IdleToJumping->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromIdle);
	IdleToJumping->Init(StateJumping);

	// Idle TO Crouching
	TObjectPtr<UTransition> IdleToCrouching = NewObject<UTransition>();
	StateIdle->Transitions.Add(IdleToCrouching);
	IdleToCrouching->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanCrouchFromIdle);
	IdleToCrouching->Init(StateCrouching);

	// Idle TO Running
	TObjectPtr<UTransition> IdleToRunning = NewObject<UTransition>();
	StateIdle->Transitions.Add(IdleToRunning);
	IdleToRunning->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanRunFromIdle);
	IdleToRunning->Init(StateRunning);

	// Idle TO Mantle
	TObjectPtr<UTransition> IdleToMantle = NewObject<UTransition>();
	StateIdle->Transitions.Add(IdleToMantle);
	IdleToMantle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanMantleFromAny);
	IdleToMantle->Init(StateMantle);

	// Idle TO Vault
	TObjectPtr<UTransition> IdleToVault = NewObject<UTransition>();
	StateIdle->Transitions.Add(IdleToVault);
	IdleToVault->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanVaultFromAny);
	IdleToVault->Init(StateVault);

	// WALKING
	// Walking TO Idle
	TObjectPtr<UTransition> WalkingToIdle = NewObject<UTransition>();
	StateWalking->Transitions.Add(WalkingToIdle);
	WalkingToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromWalk);
	WalkingToIdle->Init(StateIdle);
	
	// Walking TO Running
	TObjectPtr<UTransition> WalkingToRunning = NewObject<UTransition>();
	StateWalking->Transitions.Add(WalkingToRunning);
	WalkingToRunning->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanRunFromWalk);
	WalkingToRunning->Init(StateRunning);

	// Walking TO Crouching
	TObjectPtr<UTransition> WalkingToCrouching = NewObject<UTransition>();
	StateWalking->Transitions.Add(WalkingToCrouching);
	WalkingToCrouching->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanCrouchFromWalk);
	WalkingToCrouching->Init(StateCrouching);

	// Walking TO Jump
	TObjectPtr<UTransition> WalkingToJump = NewObject<UTransition>();
	StateWalking->Transitions.Add(WalkingToJump);
	WalkingToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromWalk);
	WalkingToJump->Init(StateJumping);

	// Walking TO Mantle
	TObjectPtr<UTransition> WalkingToMantle = NewObject<UTransition>();
	StateWalking->Transitions.Add(WalkingToMantle);
	WalkingToMantle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanMantleFromAny);
	WalkingToMantle->Init(StateMantle);

	// Walking TO Vault
	TObjectPtr<UTransition> WalkingToVault = NewObject<UTransition>();
	StateWalking->Transitions.Add(WalkingToVault);
	WalkingToVault->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanVaultFromAny);
	WalkingToVault->Init(StateVault);

	// Walking TO Dash
	TObjectPtr<UTransition> WalkingToDash = NewObject<UTransition>();
	StateWalking->Transitions.Add(WalkingToDash);
	WalkingToDash->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanDashFromAny);
	WalkingToDash->Init(StateDash);

	// RUNNING
	// Running TO Idle
	TObjectPtr<UTransition> RunningToIdle = NewObject<UTransition>();
	StateRunning->Transitions.Add(RunningToIdle);
	RunningToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromRun);
	RunningToIdle->Init(StateIdle);

	// Running TO Walking
	TObjectPtr<UTransition> RunningToWalking = NewObject<UTransition>();
	StateRunning->Transitions.Add(RunningToWalking);
	RunningToWalking->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanWalkFromRun);
	RunningToWalking->Init(StateWalking);

	// Running TO Jump
	TObjectPtr<UTransition> RunningToJump = NewObject<UTransition>();
	StateRunning->Transitions.Add(RunningToJump);
	RunningToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromRun);
	RunningToJump->Init(StateJumping);

	// Running TO Mantle
	TObjectPtr<UTransition> RunningToMantle = NewObject<UTransition>();
	StateRunning->Transitions.Add(RunningToMantle);
	RunningToMantle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanMantleFromAny);
	RunningToMantle->Init(StateMantle);

	// Running TO Vault
	TObjectPtr<UTransition> RunningToVault = NewObject<UTransition>();
	StateRunning->Transitions.Add(RunningToVault);
	RunningToVault->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanVaultFromAny);
	RunningToVault->Init(StateVault);

	// Running TO Dash
	TObjectPtr<UTransition> RunningToDash = NewObject<UTransition>();
	StateRunning->Transitions.Add(RunningToDash);
	RunningToDash->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanDashFromAny);
	RunningToDash->Init(StateDash);

	// JUMP
	// Jump TO Idle
	TObjectPtr<UTransition> JumpToIdle = NewObject<UTransition>();
	StateJumping->Transitions.Add(JumpToIdle);
	JumpToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromJump);
	JumpToIdle->Init(StateIdle);

	// Jump TO Walking
	TObjectPtr<UTransition> JumpToWalking = NewObject<UTransition>();
	StateJumping->Transitions.Add(JumpToWalking);
	JumpToWalking->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanWalkFromJump);
	JumpToWalking->Init(StateWalking);

	// Jump TO Running
	TObjectPtr<UTransition> JumpToRunning = NewObject<UTransition>();
	StateJumping->Transitions.Add(JumpToRunning);
	JumpToRunning->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanRunFromJump);
	JumpToRunning->Init(StateRunning);

	// Jump TO Crouch
	TObjectPtr<UTransition> JumpToCrouch = NewObject<UTransition>();
	StateJumping->Transitions.Add(JumpToCrouch);
	JumpToCrouch->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanCrouchFromJump);
	JumpToCrouch->Init(StateCrouching);

	// Jump TO Mantle
	TObjectPtr<UTransition> JumpToMantle = NewObject<UTransition>();
	StateJumping->Transitions.Add(JumpToMantle);
	JumpToMantle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::TryMantle);
	JumpToMantle->Init(StateMantle);

	// Jump TO Jump
	TObjectPtr<UTransition> JumpToJump = NewObject<UTransition>();
	StateJumping->Transitions.Add(JumpToJump);
	JumpToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromJump);
	JumpToJump->Init(StateJumping);
	
	// CROUCH
	// Crouch TO Idle
	TObjectPtr<UTransition> CrouchToIdle = NewObject<UTransition>();
	StateCrouching->Transitions.Add(CrouchToIdle);
	CrouchToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromCrouch);
	CrouchToIdle->Init(StateIdle);

	// Crouch TO Walking
	TObjectPtr<UTransition> CrouchToWalking = NewObject<UTransition>();
	StateCrouching->Transitions.Add(CrouchToWalking);
	CrouchToWalking->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanWalkFromCrouch);
	CrouchToWalking->Init(StateWalking);

	// Crouch TO Running
	TObjectPtr<UTransition> CrouchToRunning = NewObject<UTransition>();
	StateCrouching->Transitions.Add(CrouchToRunning);
	CrouchToRunning->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanRunFromCrouch);
	CrouchToRunning->Init(StateRunning);

	// Crouch TO Jump
	TObjectPtr<UTransition> CrouchToJump = NewObject<UTransition>();
	StateCrouching->Transitions.Add(CrouchToJump);
	CrouchToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromCrouch);
	CrouchToJump->Init(StateJumping);

	// MANTLE
	// Mantle TO Idle
	TObjectPtr<UTransition> MantleToJump = NewObject<UTransition>();
	StateMantle->Transitions.Add(MantleToJump);
	MantleToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromMantle);
	MantleToJump->Init(StateIdle);

	// VAULT
	// Vault TO Idle
	TObjectPtr<UTransition> VaultToIdle = NewObject<UTransition>();
	StateVault->Transitions.Add(VaultToIdle);
	VaultToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromVault);
	VaultToIdle->Init(StateIdle);

	// DASH
	// Dash TO Idle
	TObjectPtr<UTransition> DashToIdle = NewObject<UTransition>();
	StateDash->Transitions.Add(DashToIdle);
	DashToIdle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromDash);
	DashToIdle->Init(StateIdle);
}

void UCustomCharacterMovementComponent::SetCurrentMovementState(ECustomMovementState NewState)
{
	CurrentMovementState = NewState;
}

void UCustomCharacterMovementComponent::SetLastMovementState(ECustomMovementState NewState)
{
	LastMovementState = NewState;
}

// STATE MACHINE CONDITION CHECKER METHODS
// These methods are used by the state machine to check if a state can transition to another one and under what conditions
// FROM IDLE
bool UCustomCharacterMovementComponent::CanWalkFromIdle() const
{
	return IsMovingOnGround() && !Velocity.IsZero();
}

bool UCustomCharacterMovementComponent::CanRunFromIdle() const
{
	return bWantsToRun && !Velocity.IsZero();
}

bool UCustomCharacterMovementComponent::CanCrouchFromIdle() const
{
	return bWantsToCrouchCustom;
}

bool UCustomCharacterMovementComponent::CanJumpFromIdle()
{
	return bWantsToJump && !TryVault() && !TryMantle();
}

// FROM WALKING
bool UCustomCharacterMovementComponent::CanIdleFromWalk() const
{
	return IsMovingOnGround() && Velocity.IsZero();
}

bool UCustomCharacterMovementComponent::CanRunFromWalk() const
{
	return IsMovingOnGround() && bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanCrouchFromWalk() const
{
	return IsMovingOnGround() && bWantsToCrouchCustom;
}

bool UCustomCharacterMovementComponent::CanJumpFromWalk()
{
	return bWantsToJump && !TryVault() && !TryMantle();
}

// FROM RUNNING
bool UCustomCharacterMovementComponent::CanIdleFromRun() const
{
	return IsMovingOnGround() && Velocity.IsZero() && !bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanWalkFromRun() const
{
	return IsMovingOnGround() && !bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanJumpFromRun()
{
	return bWantsToJump && !TryVault() && !TryMantle();
}

// FROM JUMP
bool UCustomCharacterMovementComponent::CanIdleFromJump() const
{
	return IsMovingOnGround() && Velocity.IsZero() && !bWantsToJump;
}

bool UCustomCharacterMovementComponent::CanWalkFromJump() const
{
	return IsMovingOnGround() && !bWantsToJump;
}

bool UCustomCharacterMovementComponent::CanRunFromJump() const
{
	return IsMovingOnGround() && !Velocity.IsZero() && bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanCrouchFromJump() const
{
	return IsMovingOnGround() && bWantsToCrouchCustom && !bWantsToJump;
}

bool UCustomCharacterMovementComponent::CanJumpFromJump() const
{
	return bCanDoubleJump && bWantsToJump && MovementMode == MOVE_Falling;
}

// FROM CROUCH
bool UCustomCharacterMovementComponent::CanIdleFromCrouch()
{
	return IsMovingOnGround() && Velocity.IsZero() && !bWantsToCrouchCustom && CanUncrouch();
}

bool UCustomCharacterMovementComponent::CanWalkFromCrouch()
{
	return IsMovingOnGround() && !Velocity.IsZero() && !bWantsToCrouchCustom && CanUncrouch();
}

bool UCustomCharacterMovementComponent::CanRunFromCrouch()
{
	return IsMovingOnGround() && bWantsToRun && CanUncrouch();
}

bool UCustomCharacterMovementComponent::CanJumpFromCrouch()
{
	return bWantsToJump && CanUncrouch();
}

// FROM MANTLE
bool UCustomCharacterMovementComponent::CanIdleFromMantle() const
{
	return !bCanMantle;
}

// FROM VAULT
bool UCustomCharacterMovementComponent::CanIdleFromVault() const
{
	return !bCanVault;
}

// FROM DASH
bool UCustomCharacterMovementComponent::CanIdleFromDash() const
{
	return !bWantsToDash;
}

// FROM ANY
bool UCustomCharacterMovementComponent::CanMantleFromAny() const
{
	return bCanMantle && !bCanVault;
}

bool UCustomCharacterMovementComponent::CanVaultFromAny() const
{
	return bCanVault && !bCanMantle;
}

bool UCustomCharacterMovementComponent::CanDashFromAny() const
{
	return bWantsToDash;
}


// Used to check if there is enough space over the player when trying to exit from the CROUCH state
bool UCustomCharacterMovementComponent::CanUncrouch()
{
	FVector BaseLocation = UpdatedComponent->GetComponentLocation() + ((GetCapsuleHalfHeight() - 10) * FVector::UpVector);
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(GetCapsuleRadius(), MainCharacter->GetUncrouchedCapsuleHalfHeight());
	auto Params = MainCharacter->GetIgnoreCharacterParams();
	if (GetWorld()->OverlapAnyTestByProfile(BaseLocation, FQuat::Identity, "BlockAll", CapsuleShape, Params))
	{
		bWantsToCrouchCustom = true;
		return false;
	}
	return true;
}
#pragma endregion STATE_MACHINE

// Called every frame
void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Each tick the state machine checks if, for the current state a transition has become true and eventually performs the transition towards that state
	StateMachine->Tick();
}

// CURRENT STATE GETTER
ECustomMovementState UCustomCharacterMovementComponent::GetCurrentMovementState() const
{
	return CurrentMovementState;
}

ECustomMovementState UCustomCharacterMovementComponent::GetLastMovementState() const
{
	return LastMovementState;
}

float UCustomCharacterMovementComponent::GetCapsuleRadius() const
{
	return MainCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
}

float UCustomCharacterMovementComponent::GetCapsuleHalfHeight() const
{
	return MainCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

// MOVEMENT
#pragma region MOVEMENT

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

void UCustomCharacterMovementComponent::CrouchPressed()
{
	if (CurrentMovementState == ECustomMovementState::Jumping || CurrentMovementState == ECustomMovementState::Running) return;
	bWantsToCrouchCustom = !bWantsToCrouchCustom;
}

void UCustomCharacterMovementComponent::DashPressed()
{
	if (CurrentMovementState == ECustomMovementState::Crouching || CurrentMovementState == ECustomMovementState::Jumping) return;
	bWantsToDash = !bWantsToDash;
}

#pragma endregion

// MANTLE SYSTEM
// The mantle system is used to check if (when jumping/in the jump state) there is an object in front of the character over which he can climb and cannot vault over
// It uses various raycast to find the object, its height and potential steepness, and if there is enough space over it for the player
// Lastly it differentiates between a high/low mantle and set a boolean variable to true for the custom state machine to perform the transition
// The implementation for the mantle is done in blueprint since I use a timeline to perform the movement while the setup for that is done in the "StateMantle.cpp"
#pragma region MANTLE

bool UCustomCharacterMovementComponent::TryMantle()
{
	if (bCanVault) return false;

	// Location of the base of the capsule
	FVector BaseLocation = UpdatedComponent->GetComponentLocation() + FVector::DownVector * GetCapsuleHalfHeight();
	// Forward vector
	FVector Forward = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
	// Actors to ignore
	auto Params = MainCharacter->GetIgnoreCharacterParams();
	// Max height reachable by the mantle
	float MaxHeight = GetCapsuleHalfHeight() * 2 + MantleReachHeight;
	
	float CosMantleMinWallSteepnessAngle = FMath::Cos(FMath::DegreesToRadians(MantleMinWallSteepnessAngle));
	float CosMantleMaxSurfaceAngle = FMath::Cos(FMath::DegreesToRadians(MantleMaxSurfaceAngle));
	float CosMantleMaxAlignmentAngle = FMath::Cos(FMath::DegreesToRadians(MantleMaxAlignmentAngle));

	// CHECK OBSTACLE FRONT FACE
	
	FHitResult FrontHit;

	// Scale the distance in which we check for a possible hit with the velocity of the character 
	//float CheckDistance = FMath::Clamp(Velocity | Forward, GetCapsuleRadius() + 30, MantleMaxDistance);
	float CheckDistance = GetCapsuleRadius() + MantleMaxDistance;
	// The starting point of the line trace takes into account the step height and a customisable offset
	FVector FrontStart = BaseLocation + FVector::UpVector * (MaxStepHeight - 1);
	FrontStart.Z += MantleBaseStartOffset;

	for (int i = 0; i < 10; i++)
	{
		if (GetWorld()->LineTraceSingleByProfile(FrontHit, FrontStart, FrontStart + Forward * CheckDistance, "BlockAll", Params)) break;
		FrontStart += FVector::UpVector * (2.f * GetCapsuleHalfHeight() - (MaxStepHeight - 1)) / 9;
	}
	
	if (!FrontHit.IsValidBlockingHit()) return false;
	
	float CosWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	// Check if the front of the object is too steep
	if (FMath::Abs(CosWallSteepnessAngle) > CosMantleMinWallSteepnessAngle || (Forward | -FrontHit.Normal) <  CosMantleMaxAlignmentAngle) return false;

	DRAW_POINT(FrontHit.Location, FColor::Red)

	// CHECK OBSTACLE HEIGHT
	
	TArray<FHitResult> HeightHits;
	FHitResult SurfaceHit; 

	// Project the UP vector onto the normal vector of the object hit and normalize it
		// This give us a vector that goes UP in the direction of the wall
	FVector WallUpVector = FVector::VectorPlaneProject(FVector::UpVector, FrontHit.Normal).GetSafeNormal();
	float WallCos = FVector::UpVector | FrontHit.Normal;
	float WallSin = FMath::Sqrt(1 - WallCos * WallCos);

	// The starting point for the line trace is the location of the wall + a forward vector scaled by the WALL-UP vector times the magnitude we want this vector (the max height we can perform the mantle), all divided by WALL-SIN
		// WALL-SIN will be between 0 and 1 thus making TRACE-START bigger to account for the potential steepness of the wall (to ensure that TRACE-START is always of the same length)
	FVector TraceStart = FrontHit.Location + Forward + WallUpVector * (MaxHeight - (MaxStepHeight - 1)) / WallSin;
	DRAW_POINT(TraceStart, FColor::Yellow);
	DRAW_LINE(TraceStart, FrontHit.Location + Forward, FColor::Orange);

	if(!GetWorld()->LineTraceMultiByProfile(HeightHits, TraceStart, FrontHit.Location + Forward, "BlockAll", Params)) return false;
	for(const FHitResult& Hit : HeightHits)
	{
		// The tag is for the level designer to specify specific objects that should not be "mantable"
		if (Hit.IsValidBlockingHit() && !Hit.GetActor()->ActorHasTag("NotMantle"))
		{
			SurfaceHit = Hit;
			break;
		}
	}

	// Check if the surface hit is valid OR if the surface is too steep to perform the mantle
	if (!SurfaceHit.IsValidBlockingHit() || (SurfaceHit.Normal | FVector::UpVector) < CosMantleMaxSurfaceAngle) return false;

	// We can consider the value of the dot product as the component of the first vector over the second one, thus giving us the height of the surface from the base location
	float Height = (SurfaceHit.Location - BaseLocation) | FVector::UpVector;

	DRAW_POINT(SurfaceHit.Location, FColor::Blue);

	if (Height > MaxHeight) return false;

	// CHECK CLEARANCE

	float SurfaceCos = FVector::UpVector | SurfaceHit.Normal;
	float SurfaceSin = FMath::Sqrt(1 - SurfaceCos * SurfaceCos);

	// The point the capsule should mantle to, it takes into account the size of the capsule and any potential steepness of the surface
	FVector ClearanceCapsuleLocation = SurfaceHit.Location + Forward * GetCapsuleRadius() + FVector::UpVector * (GetCapsuleHalfHeight() + 1 + GetCapsuleRadius() * 2 * SurfaceSin);
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(GetCapsuleRadius(), GetCapsuleHalfHeight());

	if (GetWorld()->OverlapAnyTestByProfile(ClearanceCapsuleLocation, FQuat::Identity, "BlockAll", CapsuleShape, Params))
	{
		DRAW_CAPSULE(ClearanceCapsuleLocation, FColor::Red)
		return false;
	}

	// Check if there are any obstacles between the player and the final point
	if (GetWorld()->OverlapAnyTestByProfile(GetActorLocation() + FVector::UpVector * (GetCapsuleHalfHeight() + 1 + GetCapsuleRadius() * 2 * SurfaceSin), FQuat::Identity, "BlockAll", CapsuleShape, Params))
	{
		DRAW_CAPSULE(ClearanceCapsuleLocation, FColor::Black)
		return false;
	}
	
	DRAW_CAPSULE(SurfaceHit.Location + Forward * GetCapsuleRadius() + FVector::UpVector * GetCapsuleHalfHeight(), FColor::Green)
	
	MantleLocation = SurfaceHit.Location + Forward * GetCapsuleRadius() + FVector::UpVector * GetCapsuleHalfHeight();
	bCanMantle = true;
	bHighMantle = true;

	if (Height < LowMantleCutoff) bHighMantle = false;
	
	return true;
}
#pragma endregion

// VAULT SYSTEM
// The vault system is used to check if (when jumping) there is an object in front of the character over which he can vault
// It uses various raycast (as for the mantle system) to find the object, its height, if there is enough space over it and most importantly if there is enough space on the other side of the object
// It differentiates between a normal vault and a "falling" one:
// - Normal vault: If on the other side of the object the ground is at a similar height than the starting side
// - Falling vault: If a normal one is not possible, this can be achieved only if the player runs or walks towards the obstacle, if it tries to vault from an IDLE state then a classic mantle would be performed
// The implementation for the vault is done in blueprint since I use a timeline to perform the movement while the setup for that is done in the "StateVault.cpp"
#pragma region VAULT

bool UCustomCharacterMovementComponent::TryVault()
{
	// Location of the base of the capsule
	FVector BaseLocation = UpdatedComponent->GetComponentLocation() + FVector::DownVector * GetCapsuleHalfHeight();
	// Forward vector
	FVector Forward = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
	// Actors to ignore
	auto Params = MainCharacter->GetIgnoreCharacterParams();

	FHitResult FrontHit;

	// Scale the distance in which we check for a possible hit with the velocity of the character
	float CheckDistance = FMath::Clamp(Velocity | Forward, GetCapsuleRadius() + 30, VaultMaxDistanceCheck);
	
	// The starting point of the line trace takes into account the step height and a customisable offset
	FVector FrontStart = BaseLocation + FVector::UpVector * (MaxStepHeight - 1);

	for (int i = 0; i < 10; i++)
	{
		if (GetWorld()->LineTraceSingleByProfile(FrontHit, FrontStart, FrontStart + Forward * CheckDistance, "BlockAll", Params)) break;
		FrontStart += FVector::UpVector * (GetCapsuleHalfHeight() - (MaxStepHeight - 1)) / 6;
	}

	if (!FrontHit.IsValidBlockingHit()) return false;

	DRAW_POINT(FrontHit.Location, FColor::Red)

	// CHECK OBSTACLE HEIGHT
	
	TArray<FHitResult> HeightHits;
	FHitResult SurfaceHit; 

	// Project the UP vector onto the normal vector of the object hit and normalize it
		// This give us a vector that goes UP in the direction of the wall
	FVector WallUpVector = FVector::VectorPlaneProject(FVector::UpVector, FrontHit.Normal).GetSafeNormal();

	FVector TraceStart = FrontHit.Location + Forward + WallUpVector * (VaultMaxPossibleHeight - (MaxStepHeight - 1));
	DRAW_POINT(TraceStart, FColor::Yellow)
	DRAW_LINE(TraceStart, FrontHit.Location + Forward, FColor::Orange)

	if(!GetWorld()->LineTraceMultiByProfile(HeightHits, TraceStart, FrontHit.Location + Forward, "BlockAll", Params)) return false;
	for(const FHitResult& Hit : HeightHits)
	{
		// The tag is for the level designer to specify specific objects that should not be "vaultable"
		if (Hit.IsValidBlockingHit() && !Hit.GetActor()->ActorHasTag("NotVault"))
		{
			SurfaceHit = Hit;
			break;
		}
	}

	float Height = (SurfaceHit.Location - BaseLocation) | FVector::UpVector;

	DRAW_POINT(SurfaceHit.Location, FColor::Blue);

	if (Height > VaultMaxPossibleHeight) return false;

	// CHECK CLEARANCE

	// The point the capsule should pass while vaulting
	FVector ClearanceCapsuleLocation = SurfaceHit.Location + Forward * GetCapsuleRadius() + FVector::UpVector * GetCapsuleHalfHeight();
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(GetCapsuleRadius(), GetCapsuleHalfHeight());

	if (GetWorld()->OverlapAnyTestByProfile(ClearanceCapsuleLocation, FQuat::Identity, "BlockAll", CapsuleShape, Params))
	{
		DRAW_CAPSULE(ClearanceCapsuleLocation, FColor::Red)
		return false;
	}

	// Check if there are any obstacles between the player and the final point
	if (GetWorld()->OverlapAnyTestByProfile(GetActorLocation() + FVector::UpVector * (GetCapsuleHalfHeight()), FQuat::Identity, "BlockAll", CapsuleShape, Params))
	{
		DRAW_CAPSULE(ClearanceCapsuleLocation, FColor::Black)
		return false;
	}
	
	FVector CapsuleFinalLocationOffset = SurfaceHit.Location + Forward * (GetCapsuleRadius() + VaultMaxPossibleWidth) + FVector::UpVector * (GetCapsuleHalfHeight() / 2);

	// Check if there is enough space on the other side of the obstacle
	if (GetWorld()->OverlapAnyTestByProfile(CapsuleFinalLocationOffset, FQuat::Identity, "BlockAll", CapsuleShape, Params))
	{
		DRAW_CAPSULE(CapsuleFinalLocationOffset, FColor::Silver)
		return false;
	}

	// If on the other side of the object the ground is at a similar height than the starting point
	FHitResult GroundHit;
	TArray<AActor*> ActorsToIgnore;
	VaultMiddleLocation = ClearanceCapsuleLocation - Forward * GetCapsuleRadius();

	if (UKismetSystemLibrary::CapsuleTraceSingleByProfile(GetWorld(), CapsuleFinalLocationOffset, CapsuleFinalLocationOffset + FVector::DownVector * (GetCapsuleHalfHeight() / 2  + 15), GetCapsuleRadius(), GetCapsuleHalfHeight(), "BlockAll", false, ActorsToIgnore, EDrawDebugTrace::None ,GroundHit, true))
	{
		bFallingVault = false;
		DRAW_CAPSULE(VaultMiddleLocation, FColor::Purple)
		VaultLocation = GroundHit.Location;
		DRAW_CAPSULE(VaultLocation, FColor::Green)
	}
	else
	{
		if (Velocity.IsZero()) return false;
		bFallingVault = true;
		VaultLocation = CapsuleFinalLocationOffset;
		DRAW_CAPSULE(VaultLocation, FColor::Blue)
	}
	
	bCanVault = true;
	return true;
}

#pragma endregion 