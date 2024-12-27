// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Character/Character_Muriel.h"

#include "EnhancedInputComponent.h"
#include "AnimInstance_Muriel.h"

ACharacter_Muriel::ACharacter_Muriel()
{
	PrimaryActorTick.bCanEverTick = true;

	Data.RoleType = ERoleType::Healer;
	Data.Name = "Muriel";
	Data.Team = false;
	Data.MaxHp = 200.0f;
	Data.Hp = 200.0f;
	Data.Shield = 0.0f;
	Data.Speed = 400.0f;
	Data.Power = 15.0f;
}

void ACharacter_Muriel::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UAnimInstance_Muriel>(GetMesh()->GetAnimInstance());
}

void ACharacter_Muriel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacter_Muriel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter_Muriel::MurielJump);
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Completed, this, &ACharacter_Muriel::StopJumping);
		
		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Muriel::MurielLShift);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielLShift);

		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Triggered, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielRMBEnd);
	}
}

void ACharacter_Muriel::ChangeAttackState(EAttackState state)
{
	switch (state)
	{
	case EAttackState::QSkill:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QSkill"));
		// 쿨타임
		//if (QSkillCoolTime > 0)
		//{
		//	return;
		//}
		//QSkillCoolTime = 60.0f;
		// Montage
		PlayMontage("Q", 1.0f);
		// Effect
		break;
	case EAttackState::ESkill:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ESkill"));
		// 쿨타임
		//if (ESkillCoolTime > 0)
		//{
		//	return;
		//}
		//ESkillCoolTime = 20.0f;
		//// Montage
		//PlayMontage("E", 1.0f);
		//GetCharacterMovement()->GravityScale = 0.7f;
		//Jump();
		// Effect
		break;
	case EAttackState::LMB:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("LMB"));
		// Montage
		PlayMontage("LMB", 1.0f);
		//SpawnEffect("FX_Hand_R4");

		// Effect
		break;
	case EAttackState::RMB:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("RMB"));
		// 쿨타임
		//if (RMBSkillCoolTime > 0)
		//{
		//	return;
		//}
		//RMBSkillCoolTime = 5.0f;


		// Montage
		PlayMontage("RMB", 1.0f);
		// Effect
		break;
	default:
		break;
	}
}

void ACharacter_Muriel::ChangeState(EMoveState state)
{
}

void ACharacter_Muriel::InputAttack(const FInputActionValue& inputValue)
{
	//for (auto& mon : AttackMontages)
	//{
	//	if (AnimInstance->Montage_IsPlaying(mon.Value))
	//	{
	//		return;
	//	}
	//}
	//
	//int inputVector = inputValue.Get<float>();
	//inputVector--;
	//AttackState = static_cast<EAttackState>(inputVector);
	//ChangeAttackState(AttackState);
}



void ACharacter_Muriel::MurielLShift()
{
}

void ACharacter_Muriel::MurielJump()
{
}

void ACharacter_Muriel::MurielRMBEnd()
{
}

void ACharacter_Muriel::PlayMontage(FString Key, float InPlayRate, FName StartSectionName)
{
	if (AttackMontages.Contains(Key))
	{
		// AttackMontages에 있는 몽타주가 실행중이라면 리턴
		for (auto& mon : AttackMontages)
		{
			if (AnimInstance->Montage_IsPlaying(mon.Value))
			{
				return;
			}
		}

		UAnimMontage* montage = AttackMontages[Key];
		PlayAnimMontage(montage, InPlayRate);
	}
}

void ACharacter_Muriel::UpdateCoolTime(float DeltaTime)
{
}

void ACharacter_Muriel::SpawnEffect(FName socketName, FName key)
{
}
