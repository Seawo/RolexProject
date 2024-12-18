// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Rampage.h"
#include "EnhancedInputComponent.h"
//#include <GameFramework\SpringArmComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "RampageAnimInstance.h"

ACharacter_Rampage::ACharacter_Rampage()
{
	PrimaryActorTick.bCanEverTick = true;

	Data.RoleType = ERoleType::Dealer;
	Data.Name = "Phase";
	Data.Team = true;
	Data.MaxHp = 250.0f;
	Data.Hp = 250.0f;
	Data.Shield = 0.0f;
	Data.Speed = 400.0f;
	Data.Power = 20.0f;


	
}

void ACharacter_Rampage::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACharacter_Rampage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_Rampage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter_Rampage::InputJump);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Rampage::InputRun);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Rampage::InputRun);

		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Rampage::InputAttack);
		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Rampage::InputAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Rampage::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacter_Rampage::InputAttack);
	}
}

void ACharacter_Rampage::ChangeState(EAttackState state)
{
	switch (state)
	{
	case EAttackState::QSkill:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QSkill"));
		// Montage

		// Effect
		break;
	case EAttackState::ESkill:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ESkill"));
		// Montage

		// Effect
		break;
	case EAttackState::LMB:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("LMB"));
		// Montage

// Effect
		break;
	case EAttackState::RMB:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("RMB"));
		// Montage

// Effect
		break;
	default:
		break;
	}
}

void ACharacter_Rampage::InputAttack(const FInputActionValue& inputValue)
{
	int inputVector = inputValue.Get<float>();
	inputVector--;
	EAttackState state = static_cast<EAttackState>(inputVector);
	ChangeState(state);
}

void ACharacter_Rampage::InputJump()
{
	Jump();
}

void ACharacter_Rampage::InputRun()
{
	URampageAnimInstance* anim = Cast<URampageAnimInstance>(GetMesh()->GetAnimInstance());

	if (bIsRun)
	{
		bIsRun = false;
		
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		anim->SlopeForwardAngle = FMath::Lerp(-25.0f, 25.0f, 0.5f);
	}
	else
	{
		bIsRun = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		anim->SlopeForwardAngle = FMath::Lerp(25.0f, -25.0f, 0.5f);
	}

}
