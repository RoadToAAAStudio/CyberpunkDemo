// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// Includes
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CustomCharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpecHandle.h"
#include "QuickhackSystemComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MainCharacter.generated.h"

struct FGameplayAbilitySpec;
// Forward declarations
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(config=Game)
class CYBERPUNKDEMO_API AMainCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Aiming")
	float MouseSensibility = 1;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Movement)
	UCustomCharacterMovementComponent* CustomCharacterMovementComponent;

private:
	
	// Pawn mesh: 1st person view (arms; seen only by self)
    UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
    USkeletalMeshComponent* Mesh1P;
    
    // First person camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FirstPersonCameraComponent;
	
#pragma region ABILITY_SYSTEM_COMPONENT (W_I_P)
		// Ability System Component
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ASC", meta=(AllowPrivateAccess = "true"))
		UAbilitySystemComponent* AbilitySystemComponent;

		// Quickhack System Component
		UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "QSC", meta=(AllowPrivateAccess = "true"))
		UQuickhackSystemComponent* QuickhackSystemComponent;

		UPROPERTY()
		FGameplayAbilitySpecHandle ShootSpec;
#pragma endregion

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	
	// Input Actions
	#pragma region INPUT_ACTIONS
    	/** MappingContext [!] */
    	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    	UInputMappingContext* DefaultMappingContext;
    
    	/** Jump Input Action [!] */
    	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    	UInputAction* JumpAction;
    
    	/** Move Input Action [!] */
    	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
    	UInputAction* MoveAction;

		/** Look Input Action [!] */
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	    UInputAction* LookAction;

		// Sprint action
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true") )
		UInputAction* SprintAction;

		// Crouch action
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* CrouchAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta =(AllowPrivateAccess = "true"))
		UInputAction* DashAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta =(AllowPrivateAccess = "true"))
		UInputAction* ShootAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta =(AllowPrivateAccess = "true"))
		UInputAction* AnalysisAction;
	
	#pragma endregion 

	float UncrouchedCapsuleHalfHeight;
	
public:
	
	// Sets default values for this character's properties
	AMainCharacter(const FObjectInitializer& ObjectInitializer);

	// Returns Mesh1P sub-object
	UFUNCTION(BlueprintCallable) USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	// Returns FirstPersonCameraComponent sub-object
	UFUNCTION(BlueprintCallable) UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region ABILITY_SYSTEM_COMPONENT (W_I_P)
	// Getter for the ASC
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	TSet<TSubclassOf<UGameplayAbility>> GetPlayerHacks();
#pragma endregion
	
	// Getter for the Custom Character Movement Component
	UCustomCharacterMovementComponent* GetCustomCharacterMovementComponent();

	// Method that returns all parameters that should be ignored by raycasts, spherecasts and so on
	FCollisionQueryParams GetIgnoreCharacterParams() const;

	UFUNCTION(BlueprintCallable) float GetUncrouchedCapsuleHalfHeight();

	UFUNCTION(BlueprintImplementableEvent) void Mantle();

	UFUNCTION(BlueprintImplementableEvent) void Vault();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

#pragma region INPUT_METHODS
	// Called for movement input
	void Move(const FInputActionValue& Value);

	// Called for looking input
	void Look(const FInputActionValue& Value);
#pragma endregion 
};
