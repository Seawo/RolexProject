// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Sparrow.h"
#include "EnhancedInputComponent.h"
#include <GameFramework\SpringArmComponent.h>

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components\SkeletalMeshComponent.h"
#include "SparrowAnimInstance.h"


ACharacter_Sparrow::ACharacter_Sparrow()
{

	// character setting 
	Data.RoleType = ERoleType::Dealer;
	Data.Name = "Sparrow";
	Data.Team = true;
	Data.MaxHp = 250.0f;
	Data.Hp = 250.0f;
	Data.Shield = 0.0f;
	Data.Speed = 400.0f;
	Data.Power = 20.0f;

	SpringArmComp->SetRelativeLocation(FVector(0, 10, 40));

}

void ACharacter_Sparrow::BeginPlay()
{
	Super::BeginPlay();

	ChangeState(EMoveState::Start, stateMontages[TEXT("Start")]);

}

void ACharacter_Sparrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ACharacter_Sparrow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter_Sparrow::InputJump);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Sparrow::InputRun);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Sparrow::CompleteRun);

		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Sparrow::InputAttack);
		characterInput->BindAction(IA_Q, ETriggerEvent::Triggered, this, &ACharacter_Sparrow::AimOffsetQ);
		characterInput->BindAction(IA_Q, ETriggerEvent::Completed, this, &ACharacter_Sparrow::ShootingArrowQ);

		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Sparrow::InputAttack);

		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Sparrow::InputAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Triggered, this, &ACharacter_Sparrow::AimOffsetLBM);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Completed, this, &ACharacter_Sparrow::ShootingArrowLBM);

		characterInput->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacter_Sparrow::InputAttack);
	}

}

void ACharacter_Sparrow::ChangeAttackState(EAttackState state)
{
	switch (state)
	{
	case EAttackState::QSkill:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QSkill"));
		QAttack();
		break;
	case EAttackState::ESkill:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ESkill"));
		EAttack();
		break;
	case EAttackState::LMB:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("LMB"));
		LBMAttack();
		break;
	case EAttackState::RMB:
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("RMB"));
		RBMAttack();
		break;
	default:
		break;
	}

}

void ACharacter_Sparrow::InputAttack(const FInputActionValue& inputValue)
{
	UE_LOG(LogTemp, Warning, TEXT("attack in"));

	int inputVector = inputValue.Get<float>();
	inputVector--;
	CurrAttackState = static_cast<EAttackState>(inputVector);
	ChangeAttackState(CurrAttackState);
}

void ACharacter_Sparrow::AimOffsetLBM()
{
	if (bIsCharging)
	{
		FRotator controlRotation = GetController()->GetControlRotation();

		float newPitch = controlRotation.Pitch;

		anim = Cast<USparrowAnimInstance>(GetMesh()->GetAnimInstance());
		if (anim)
		{
			anim->CameraRot = FRotator(0, 0, -newPitch);
		}

		// SpringArm 위치 조정 (선택적)
		FVector NewLocation = FMath::Lerp(SpringArmComp->GetRelativeLocation(), FVector(-100, 10, 60), 0.05f);
		SpringArmComp->SetRelativeLocation(NewLocation);
	}
}

void ACharacter_Sparrow::ShootingArrowLBM()
{
	if (!bIsCharging) return;


	if (bIsCharging)
	{
		FName sectionName = FName("fire");
		bIsCharging = false;
		PlayAnimMontage(AttackMontages[TEXT("LBM")], 1.0f, *sectionName.ToString());

		GetWorld()->GetTimerManager().SetTimer(LBMAimTimerHandle, FTimerDelegate::CreateLambda(
			[this]()
			{
				anim->CameraRot = FRotator(0, 0, 0);
				SpringArmComp->SetRelativeLocation(FVector(0, 10, 40));
				
			}),
			0.5f, false);

	}
}

void ACharacter_Sparrow::AimOffsetQ()
{
	// Q 차징
	// 위치를 정하는 부분으로 넓은 범위가 생기고 카메라가 움직이면서 위치를 잡는다

	if (bIsCharging)
	{
		//
	}
}

void ACharacter_Sparrow::ShootingArrowQ()
{
	// Q 발사
	// 발싸하는 몽타주 재생
	if (!bIsCharging) return;

	if (bIsCharging)
	{
		FName sectionName = FName("fire");
		bIsCharging = false;
		PlayAnimMontage(AttackMontages[TEXT("Q")], 1.0f, *sectionName.ToString());

		// 다시 원래대로 돌아온다
		// 모션을 다 한뒤 마지막 함수 호출로 다시할 예정
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void ACharacter_Sparrow::InputJump()
{
	Jump();
}

void ACharacter_Sparrow::InputRun()
{
	if (bIsCharging) return;

	if (!bIsRun)
	{
		bIsRun = true;

		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		
	}

}

void ACharacter_Sparrow::CompleteRun()
{
	if (bIsCharging) return;

	if (bIsRun)
	{
		bIsRun = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void ACharacter_Sparrow::LBMAttack()
{
	if (!AttackMontages[TEXT("LBM")]) return;

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
		{
			return;
		}
	}

	if (!bIsCharging)
	{
		// 차징중엔 느려지게 하고, 
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;

		FName sectionName = FName("start");
		bIsCharging = true;
		
		PlayAnimMontage(AttackMontages[TEXT("LBM")], 1.0f, *sectionName.ToString());
	}

}

void ACharacter_Sparrow::RBMAttack()
{
}

void ACharacter_Sparrow::QAttack()
{
	if (!AttackMontages[TEXT("Q")]) return;

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}

	if (!bIsCharging)
	{
		FName sectionName = FName("start");
		bIsCharging = true;

		// 움직임 막기
		GetCharacterMovement()->DisableMovement();
		
		PlayAnimMontage(AttackMontages[TEXT("Q")], 1.0f, *sectionName.ToString());
	}

}

void ACharacter_Sparrow::EAttack()
{
	if (!AttackMontages[TEXT("E")]) return;

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}

	PlayAnimMontage(AttackMontages[TEXT("E")], 1.0f);
}
