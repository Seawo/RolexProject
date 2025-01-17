// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Steel.h"
#include "EnhancedInputComponent.h"
#include <GameFramework\SpringArmComponent.h>
#include "GameFramework/CharacterMovementComponent.h"

ACharacter_Steel::ACharacter_Steel()
{
	// character setting 
	Data.RoleType = ERoleType::Dealer;
	Data.Name = "Sparrow";
	Data.Team = true;
	Data.MaxHp = 250;
	Data.Hp = 250;
	Data.Shield = 0;
	Data.Speed = 400.0f;
	Data.Power = 20;

	SpringArmComp->SetRelativeLocation(FVector(0, 10, 90));

}

void ACharacter_Steel::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacter_Steel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_Steel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter_Steel::InputJump);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Steel::InputRun);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Steel::InputRun);

		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Steel::InputAttack);
		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Steel::InputAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Steel::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacter_Steel::InputAttack);
	}
}

void ACharacter_Steel::ChangeAttackState(EAttackState state)
{
	switch (state)
	{
	case EAttackState::QSkill:
		QAttack();
		break;
	case EAttackState::ESkill:
		EAttack();
		break;
	case EAttackState::LMB:
		LMBAttack();
		break;
	case EAttackState::RMB:
		RMBAttack();
		break;
	case EAttackState::QSkill_Completed:
		break;
	case EAttackState::ESkill_Completed:
		break;
	case EAttackState::LMB_Completed:
		break;
	case EAttackState::RMB_Completed:
		break;
	default:
		break;
	}
}

void ACharacter_Steel::InputAttack(const FInputActionValue& inputValue)
{
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
		{
			return;
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("attack in"));

	int inputVector = inputValue.Get<float>();
	inputVector--;
	CurrAttackState = static_cast<EAttackState>(inputVector);
	ChangeAttackState(CurrAttackState);

}

void ACharacter_Steel::InputJump()
{
	Jump();
}

void ACharacter_Steel::InputRun()
{
	if (bIsRun)
	{
		bIsRun = false;

		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		bIsRun = true;

		GetCharacterMovement()->MaxWalkSpeed = 900.0f;
	}
}

void ACharacter_Steel::LMBAttack()
{
	if (ComboCnt < MaxCnt)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontages[TEXT("LMB")]))
		{
			return; // 공격 애니메이션이 재생 중이라면 더 이상 애니메이션을 받지 않음
		}

		ComboCnt++;

		FName sectionName = FName("Attack_" + FString::FromInt(ComboCnt));
		// AttackMontages
		PlayAnimMontage(AttackMontages[TEXT("LMB")], 1.0f, *sectionName.ToString());


		// 콤보 리셋 타이머
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, FTimerDelegate::CreateLambda([this]() { ComboCnt = 0; })
			, ComboResetDelay, false);
	}
	else
	{
		ComboCnt = 0;
	}
}

void ACharacter_Steel::RMBAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("RMB"));
}

void ACharacter_Steel::QAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Q"));
}

void ACharacter_Steel::EAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("E"));
}


