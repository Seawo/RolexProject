// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Steel.h"
#include "EnhancedInputComponent.h"
#include <GameFramework\SpringArmComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Components\CapsuleComponent.h"
#include "GameFramework/PlayerController.h"

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

	if (bIsDashing)
	{
		DashTimeElapsed += DeltaTime;

		if (DashTimeElapsed >= DashTimer)
		{
			StopEDash();
		}

		DashDirection = GetActorForwardVector();
		AddMovementInput(DashDirection, DashSpeed * DeltaTime);
	}
}

void ACharacter_Steel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter_Steel::InputJump);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Steel::InputRun);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Steel::InputRun);

		// input
		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Steel::InputAttack);
		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Steel::InputAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Steel::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacter_Steel::InputAttack);

		// Completed
		characterInput->BindAction(IA_RBM, ETriggerEvent::Completed, this, &ACharacter_Steel::RMBCompleted);
		characterInput->BindAction(IA_Q, ETriggerEvent::Completed, this, &ACharacter_Steel::QCompleted);
		
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
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}

	if (bIsShield) return;

	bIsShield = true;

	FName sectionName = FName("Start");
	PlayAnimMontage(AttackMontages[TEXT("RMB")], 1.0f, *sectionName.ToString());
}

void ACharacter_Steel::QAttack()
{
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}

	FName sectionName = FName("Ready");
	PlayAnimMontage(AttackMontages[TEXT("Q")], 1.0f, *sectionName.ToString());
}

void ACharacter_Steel::EAttack()
{
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}

	if (bIsDashing) return;

	bIsDashing = true;
	GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
	SpringArmComp->SetRelativeLocation(FVector(-200, 10, 90));

	// 몽타주 재생 하고
	FName sectionName = FName("Start");
	PlayAnimMontage(AttackMontages[TEXT("E")], 1.0f, *sectionName.ToString());

	// 충돌 처리 활성화
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACharacter_Steel::OnDashCollision);

}

void ACharacter_Steel::OnDashCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	StopEDash();

	if (OtherActor && OtherActor != this)
	{
		ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

		if (character)
		{
			if (character->Data.Team == Data.Team || character->MoveState == EMoveState::Die)
			{
				// 팀이 맞음...
			}
			else
			{
				// 추가 딜 주고
				character->ModifyHP(-40);
				
				FVector myLoc = GetActorLocation();
				FVector targetLoc = character->GetActorLocation();
				FVector dir = (targetLoc - myLoc).GetSafeNormal();
				
				
				// 힘적용
				character->LaunchCharacter(dir * 5000, true, true);
			}
		}
	}
}

void ACharacter_Steel::StopEDash()
{
	bIsDashing = false;
	DashTimeElapsed = 0.0f;

	// 기본 속도로 되돌리기
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	// End 몽타주 재생하고,
	FName sectionName = FName("End");
	PlayAnimMontage(AttackMontages[TEXT("E")], 1.0f, *sectionName.ToString());

	SpringArmComp->SetRelativeLocation(FVector(0, 10, 90));

	// 적과 층돌후 충돌 처리 비활성화
	GetCapsuleComponent()->OnComponentHit.RemoveDynamic(this, &ACharacter_Steel::OnDashCollision);
}

void ACharacter_Steel::RMBCompleted()
{
	if (!bIsShield) return;

	bIsShield = false;
	FName sectionName = FName("End");
	PlayAnimMontage(AttackMontages[TEXT("RMB")], 1.0f, *sectionName.ToString());
}

void ACharacter_Steel::QCompleted()
{

	FName sectionName = FName("Start");
	PlayAnimMontage(AttackMontages[TEXT("Q")], 1.0f, *sectionName.ToString());
}




