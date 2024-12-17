// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Components\CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

#include <GameFramework\SpringArmComponent.h>
#include "Camera\CameraComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = GetCapsuleComponent();
	capsuleComp->SetCapsuleHalfHeight(100.0f);
	capsuleComp->SetCapsuleRadius(50.0f);

	// TPS 카메라 붙이기 
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 40));
	SpringArmComp->TargetArmLength = 25.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	// Camera
	TpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	TpsCamComp->SetupAttachment(SpringArmComp);
	TpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(Imc_base, 0);
		}
	}

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (characterInput)
	{
		characterInput->BindAction(IA_VectorMove, ETriggerEvent::Triggered, this, &ABaseCharacter::InputMove);
		characterInput->BindAction(IA_Rotation, ETriggerEvent::Triggered, this, &ABaseCharacter::InputRotation);

	}
}

void ABaseCharacter::InputMove(const FInputActionValue& inputValue)
{
	FVector2D InputVector = inputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRoatation(0, Rotation.Yaw, 0);

		const FVector ForwardDir = FRotationMatrix(YawRoatation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRoatation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, InputVector.Y);
		AddMovementInput(RightDir, InputVector.X);
	}
}

void ABaseCharacter::InputRotation(const FInputActionValue& inputValue)
{
	FVector2D InputVector = inputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}

}

