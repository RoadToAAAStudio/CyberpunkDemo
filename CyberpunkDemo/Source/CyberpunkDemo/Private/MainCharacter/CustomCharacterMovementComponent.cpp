// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/CustomCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter/MainCharacter.h"
#include "Utility/States/StateIdle.h"
#include "Utility/FTransition.h"
#include "Utility/States/StateCrouching.h"
#include "Utility/States/StateJumping.h"
#include "Utility/States/StateRunning.h"
#include "Utility/States/StateWalking.h"
#include "Utility/States/UStateMantle.h"

// Shortcut macros
#if 0
float MacroDuration = 2.0f;
#define PRINT_SCREEN(x) GEngine->AddOnScreenDebugMessage(-1, MacroDuration ? MacroDuration : -1.f, FColor::Yellow, x);
#define DRAW_POINT(x, c) DrawDebugPoint(GetWorld(), x, 10, c, !MacroDuration, MacroDuration);
#define DRAW_LINE(x1, x2, c) DrawDebugLine(GetWorld(), x1, x2, c, !MacroDuration, MacroDuration);
#define DRAW_CAPSULE(x, c) DrawDebugCapsule(GetWorld(), x, GetCapsuleHalfHeight(), GetCapsuleRadius(), FQuat::Identity, c, !MacroDuration, MacroDuration);
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

	bWantsToJump = false;
	bWantsToRun = false;
	bWantsToCrouchCustom = false;
	Walk_MaxWalkSpeed = 500.0f;
	Sprint_MaxWalkSpeed = 1000.0f;
	Crouch_MaxWalkSpeed = 250.0f;
	Crouch_HalfHeight = 40.0f;
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

	TObjectPtr<UUStateMantle> StateMantle = NewObject<UUStateMantle>();
	StateMachine->AddState(StateMantle);
	StateMantle->SetOwner(this);

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

	// Idle TO Mantle
	TObjectPtr<UFTransition> IdleToMantle = NewObject<UFTransition>();
	StateIdle->Transitions.Add(IdleToMantle);
	IdleToMantle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanMantleFromAny);
	IdleToMantle->Init(StateMantle);

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

	// Walking TO Mantle
	TObjectPtr<UFTransition> WalkingToMantle = NewObject<UFTransition>();
	StateWalking->Transitions.Add(WalkingToMantle);
	WalkingToMantle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanMantleFromAny);
	WalkingToMantle->Init(StateMantle);

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

	// Running TO Mantle
	TObjectPtr<UFTransition> RunningToMantle = NewObject<UFTransition>();
	StateRunning->Transitions.Add(RunningToMantle);
	RunningToMantle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanMantleFromAny);
	RunningToMantle->Init(StateMantle);

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
	JumpToRunning->Init(StateRunning);

	// Jump TO Crouch
	TObjectPtr<UFTransition> JumpToCrouch = NewObject<UFTransition>();
	StateJumping->Transitions.Add(JumpToCrouch);
	JumpToCrouch->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanCrouchFromJump);
	JumpToCrouch->Init(StateCrouching);

	// Jump TO Mantle
	TObjectPtr<UFTransition> JumpToMantle = NewObject<UFTransition>();
	StateJumping->Transitions.Add(JumpToMantle);
	JumpToMantle->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::TryMantle);
	JumpToMantle->Init(StateMantle);

	// Jump TO Jump
	TObjectPtr<UFTransition> JumpToJump = NewObject<UFTransition>();
	StateJumping->Transitions.Add(JumpToJump);
	JumpToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanJumpFromJump);
	JumpToJump->Init(StateJumping);
	
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

	// MANTLE
	// Mantle TO Idle
	TObjectPtr<UFTransition> MantleToJump = NewObject<UFTransition>();
	StateMantle->Transitions.Add(MantleToJump);
	MantleToJump->OnCheckConditionDelegate.BindUObject(this, &UCustomCharacterMovementComponent::CanIdleFromMantle);
	MantleToJump->Init(StateIdle);
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
	return bWantsToJump && !TryMantle();
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
	return bWantsToJump && !TryMantle();
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
	return bWantsToJump && !TryMantle();
}

// FROM JUMP
bool UCustomCharacterMovementComponent::CanIdleFromJump()
{
	return IsMovingOnGround() && Velocity.IsZero() && !bWantsToJump;
}

bool UCustomCharacterMovementComponent::CanWalkFromJump()
{
	return IsMovingOnGround() && !bWantsToJump;
}

bool UCustomCharacterMovementComponent::CanRunFromJump()
{
	return IsMovingOnGround() && !Velocity.IsZero() && bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanCrouchFromJump()
{
	return IsMovingOnGround() && bWantsToCrouchCustom && !bWantsToJump;
}

// bool UCustomCharacterMovementComponent::CanMantleFromJump()
// {
// 	return TryMantle() && Velocity.Z < 0;
// }

bool UCustomCharacterMovementComponent::CanJumpFromJump()
{
	return bCanDoubleJump && bWantsToJump && MovementMode == MOVE_Falling;
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
	return IsMovingOnGround() && bWantsToRun;
}

bool UCustomCharacterMovementComponent::CanJumpFromCrouch()
{
	return bWantsToJump;
}

// FROM MANTLE
bool UCustomCharacterMovementComponent::CanIdleFromMantle()
{
	return !bCanMantle;
}

bool UCustomCharacterMovementComponent::CanMantleFromAny()
{
	return bCanMantle;
}

#pragma endregion 

// Called every frame
void UCustomCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	StateMachine->Tick();
}

// MOVEMENT
#pragma region 

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
	if (CurrentMovementState == ECustomMovementState::Jumping || CurrentMovementState == ECustomMovementState::Running) return;
	bWantsToCrouchCustom = !bWantsToCrouchCustom;
}

void UCustomCharacterMovementComponent::DashPressed()
{
	if (CurrentMovementState == ECustomMovementState::Crouching || CurrentMovementState == ECustomMovementState::Jumping) return;
	MaxWalkSpeed *= DashSpeedMultiplier;
	GEngine->AddOnScreenDebugMessage(-1, DashDuration, FColor::Emerald, "I AM DASHING");
	GetWorld()->GetTimerManager().SetTimer(DashTimer, this, &UCustomCharacterMovementComponent::ResetDashSpeed, DashDuration, false);
}

void UCustomCharacterMovementComponent::ResetDashSpeed()
{
	switch (CurrentMovementState)
	{
	case ECustomMovementState::Walking:
		MaxWalkSpeed = Walk_MaxWalkSpeed;
		break;
	case ECustomMovementState::Running:
		MaxWalkSpeed = Sprint_MaxWalkSpeed;
		break;
	}
}

#pragma endregion

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

// MANTLE SYSTEM
#pragma region 

bool UCustomCharacterMovementComponent::TryMantle()
{
	// Location of the base of the capsule
	FVector BaseLocation = UpdatedComponent->GetComponentLocation() + FVector::DownVector * GetCapsuleHalfHeight();
	// Forward vector
	// [TODO : HERE MAYBE WE NEED TO GET THE CAMERA FORWARD VECTOR]
	FVector Forward = UpdatedComponent->GetForwardVector().GetSafeNormal2D();
	// Actors to ignore
	auto Params = MainCharacter->GetIgnoreCharacterParams();
	// Max height reachable by the mantle
	float MaxHeight = GetCapsuleHalfHeight() * 2 + MantleReachHeight;
	//
	float CosMantleMinWallSteepnessAngle = FMath::Cos(FMath::DegreesToRadians(MantleMinWallSteepnessAngle));
	//
	float CosMantleMaxSurfaceAngle = FMath::Cos(FMath::DegreesToRadians(MantleMaxSurfaceAngle));
	//
	float CosMantleMaxAlignmentAngle = FMath::Cos(FMath::DegreesToRadians(MantleMaxAlignmentAngle));

	PRINT_SCREEN("Starting mantle attempt")

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
		DRAW_LINE(FrontStart, FrontStart + Forward * CheckDistance, FColor::Red)

		if (GetWorld()->LineTraceSingleByProfile(FrontHit, FrontStart, FrontStart + Forward * CheckDistance, "BlockAll", Params)) break;
		FrontStart += FVector::UpVector * (2.f * GetCapsuleHalfHeight() - (MaxStepHeight - 1)) / 9;
	}
	
	if (!FrontHit.IsValidBlockingHit()) return false;
	float CosWallSteepnessAngle = FrontHit.Normal | FVector::UpVector;
	// Check if the front of the object is too steep OR if the 
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
	DRAW_LINE(TraceStart, FrontHit.Location + Forward, FColor::Orange);

	if(!GetWorld()->LineTraceMultiByProfile(HeightHits, TraceStart, FrontHit.Location + Forward, "BlockAll", Params)) return false;
	for(const FHitResult& Hit : HeightHits)
	{
		if (Hit.IsValidBlockingHit())
		{
			SurfaceHit = Hit;
			break;
		}
	}

	// Check if the surface hit is valid OR if the surface is too steep to perform the mantle
	if (!SurfaceHit.IsValidBlockingHit() || (SurfaceHit.Normal | FVector::UpVector) < CosMantleMaxSurfaceAngle) return false;

	// We can consider the value of the dot product as the component of the first vector over the second one, thus giving us the height of the surface from the base location
	float Height = (SurfaceHit.Location - BaseLocation) | FVector::UpVector;

	PRINT_SCREEN(FString::Printf(TEXT("Height: %f"), Height));

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

	DRAW_CAPSULE(ClearanceCapsuleLocation, FColor::Green)
	
	MantleLocation = ClearanceCapsuleLocation;
	bCanMantle = true;
	bHighMantle = true;

	if (Height < LowMantleCutoff) bHighMantle = false;
	
	return true;
}




#pragma endregion 