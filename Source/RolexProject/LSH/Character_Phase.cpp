// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Character_Phase.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance_Phase.h"
#include "Animation/AnimMontage.h"

ACharacter_Phase::ACharacter_Phase()
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

void ACharacter_Phase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacter_Phase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_Phase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter_Phase::Jump);
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Completed, this, &ACharacter_Phase::StopJumping);

		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Phase::InputLShift);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Phase::InputLShift);

		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Phase::InputAttack);
		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Phase::InputAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Phase::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacter_Phase::InputAttack);

	}
}

void ACharacter_Phase::ChangeState(EAttackState state)
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
		PlayMontage("LMB",1.0f,"AAttack");
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

void ACharacter_Phase::InputAttack(const FInputActionValue& inputValue)
{
	int inputVector = inputValue.Get<float>();
	inputVector--;
	EAttackState state = static_cast<EAttackState>(inputVector);
	ChangeState(state);
}

void ACharacter_Phase::InputLShift()
{
	if (bLShift)
	{
		bLShift = false;
		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		SlopeForwardAngle = 25.0f;
	}
	else
	{
		bLShift = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		SlopeForwardAngle = 0.0f;
	}
}

void ACharacter_Phase::PlayMontage(FString Key, float InPlayRate, FName StartSectionName)
{
	if (AttackMontages.Contains(Key))
	{
		// 화면에 Key값 출력하기
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Key);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, StartSectionName.ToString());

		UAnimMontage* Montage = AttackMontages[Key];
		PlayAnimMontage(Montage, InPlayRate, StartSectionName);
	}

	//if (Montage != nullptr)
	//{
	//	//몽타주 재생하기
	//	PlayAnimMontage(Montage, InPlayRate, StartSectionName);
	//
	//	//UAnimInstance_Phase* animInstance = Cast<UAnimInstance_Phase>(GetMesh()->GetAnimInstance());
	//	//animInstance->Montage_Play(Montage, 1);
	//}
}
