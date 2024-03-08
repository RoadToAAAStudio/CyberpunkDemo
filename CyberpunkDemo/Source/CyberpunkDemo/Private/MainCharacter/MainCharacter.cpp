// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "CyberpunkDemo/CyberpunkDemoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "MainCharacter/CustomPlayerController.h"

// Sets default values and create the components
AMainCharacter::AMainCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	CustomCharacterMovementComponent = Cast<UCustomCharacterMovementComponent>(GetCharacterMovement());
	
	// Set the booleans for the movement system to false
	bIsCrouching = false;
	bIsRunning = false;

	// Set size for a collision capsule [!]
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	// Create a camera component [!]
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 70));

	// Create a mesh component [!]
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->CastShadow = false;
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context [!]
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	JumpMaxCount = 2;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::Jump()
{
	Super::Jump();
	//LaunchCharacter(FVector(0, 0, 500), false, false);
}

TObjectPtr<UCustomCharacterMovementComponent> AMainCharacter::GetCustomCharacterComponent()
{
	return CustomCharacterMovementComponent;
}

FCollisionQueryParams AMainCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);
	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void AMainCharacter::DisableMappingContext(bool Enable)
{
	if (Subsystem)
	{
		if (Enable)
		{
			Subsystem->RemoveMappingContext(DefaultMappingContext);
			//Subsystem->AddMappingContext(EmptyMappingContext, 0);
		}
		else
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// void AMainCharacter::Crouch(bool bClientSimulation)
// {
// 	//Super::Crouch(bClientSimulation);
// }

// Called to bind functionality to input [!]
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, CustomCharacterMovementComponent, &UCustomCharacterMovementComponent::JumpPressed);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, CustomCharacterMovementComponent, &UCustomCharacterMovementComponent::JumpReleased);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, CustomCharacterMovementComponent, &UCustomCharacterMovementComponent::SprintPressed);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, CustomCharacterMovementComponent, &UCustomCharacterMovementComponent::SprintReleased);

		// Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, CustomCharacterMovementComponent, &UCustomCharacterMovementComponent::CrouchPressed);

		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, CustomCharacterMovementComponent, &UCustomCharacterMovementComponent::DashPressed);
	}
}

// Called to apply the direction to the movement [!]
void AMainCharacter::Move(const FInputActionValue& Value)
{
	FVector2d MovementVector = Value.Get<FVector2d>();

	if (Controller != nullptr)
	{
		// add movement
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

// Called to add the input rotation to the controller [!]
void AMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2d LookAxisVector = Value.Get<FVector2d>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * MouseSensibility);
		AddControllerPitchInput(LookAxisVector.Y * MouseSensibility);
	}
}



