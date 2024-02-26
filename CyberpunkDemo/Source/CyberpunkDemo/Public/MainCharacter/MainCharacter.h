// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// Includes
#include "CoreMinimal.h"
#include "CustomCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MainCharacter.generated.h"

// Forward declarations
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS(config=Game)
class CYBERPUNKDEMO_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Movement")
	bool bIsCrouching;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= Movement)
	UCustomCharacterMovementComponent* CustomCharacterMovementComponent;

private:
	
	/** Pawn mesh: 1st person view (arms; seen only by self) [!] */
    	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
    	USkeletalMeshComponent* Mesh1P;
    
    	/** First person camera [!] */
    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    	UCameraComponent* FirstPersonCameraComponent;
    
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

		//UPROPERTY(EditAnywhere)

public:
	
	// Sets default values for this character's properties
	AMainCharacter(const FObjectInitializer& ObjectInitializer);

	/** Returns Mesh1P sub-object [!] **/
	UFUNCTION(BlueprintCallable) USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent sub-object [!] **/
	UFUNCTION(BlueprintCallable) UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// // Called to bind functionality to input
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input [!] */
	void Move(const FInputActionValue& Value);

	/** Called for looking input [!] */
	void Look(const FInputActionValue& Value);

	// APawn interface [!]
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
};
