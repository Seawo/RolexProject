// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Rampage.h"
#include "EnhancedInputComponent.h"
//#include <GameFramework\SpringArmComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "RampageAnimInstance.h"

#include "Animation\AnimInstance.h"
#include "Animation\AnimMontage.h"

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


	// combo_attack
	MaxCnt = 5;
	ComboCnt = 0;
	ComboResetDelay = 1.5f;

}

void ACharacter_Rampage::BeginPlay()
{
	Super::BeginPlay();

	
}

void ACharacter_Rampage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDashing)
	{
		// Update elapsed time
		ElapsedTime += DeltaTime;

		// Calculate progress (0 to 1) based on elapsed time
		float Progress = ElapsedTime / DashDuration;

		// End dash if progress reaches or exceeds 1
		if (Progress >= 2.0f)
		{
			bIsDashing = false;

			// Re-enable normal movement
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

			return;
		}

		// Calculate horizontal movement
		float horizontalDistance = DashSpeed * Progress * DashDuration;

		// Calculate vertical offset (parabolic arc)
		float verticalOffset = DashHeight * FMath::Sin(PI * Progress);

		// Update character position
		FVector newLocation = StartPos + DashDirection * horizontalDistance;
		newLocation.Z += verticalOffset;

		// Apply new location
		SetActorLocation(newLocation, true);
	}



	//if (bIsDashing)
	//{
	//	// Move the character in the dash direction
	//	AddActorWorldOffset(DashDirection * DashSpeed * DeltaTime, true);
	//}
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
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QSkill"));
		QAttack();
		break;
	case EAttackState::ESkill:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ESkill"));
		EAttack();
		break;
	case EAttackState::LMB:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("LMB"));
		LBMAttack();
		break;
	case EAttackState::RMB:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("RMB"));
		RBMAttack();
		break;
	default:
		break;
	}
}

void ACharacter_Rampage::InputAttack(const FInputActionValue& inputValue)
{
	int inputVector = inputValue.Get<float>();
	inputVector--;
	CurrAttackState = static_cast<EAttackState>(inputVector);
	ChangeState(CurrAttackState);
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
		
		GetCharacterMovement()->MaxWalkSpeed = 800.0f;
		anim->SlopeForwardAngle = 3;
	}
	else
	{
		bIsRun = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		anim->SlopeForwardAngle = 0;
	}

}

void ACharacter_Rampage::LBMAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("LBM attack"));

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}


	if (ComboCnt < MaxCnt)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontages[TEXT("LBM")]))
		{
			return; // 공격 애니메이션이 재생 중이라면 더 이상 공격을 받지 않음
		}


		ComboCnt++;

		FName sectionName = FName("Attack_" + FString::FromInt(ComboCnt));
		// AttackMontages
		PlayAnimMontage(AttackMontages[TEXT("LBM")], 1.0f, *sectionName.ToString());

		//UE_LOG(LogTemp, Warning, TEXT("Playing section : %s"), *sectionName.ToString());

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

void ACharacter_Rampage::RBMAttack()
{
	if (!AttackMontages[TEXT("RBM")]) return;
	
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}



	PlayAnimMontage(AttackMontages[TEXT("RBM")], 0.8f);
}

void ACharacter_Rampage::QAttack()
{
	if (!AttackMontages[TEXT("Q")]) return;


	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}


	PlayAnimMontage(AttackMontages[TEXT("Q")], 1.0f);
}

void ACharacter_Rampage::EAttack()
{
	if (!AttackMontages[TEXT("E")]) return;

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}

	PlayAnimMontage(AttackMontages[TEXT("E")], 1.0f);

	if (bIsDashing)
		return; 

	bIsDashing = true;
	ElapsedTime = 0.0f;

	// Record the starting position and direction
	StartPos = GetActorLocation();
	DashDirection = GetActorForwardVector();

	// Disable normal movement during the dash
	GetCharacterMovement()->DisableMovement();

	// Set a timer to end the dash
	GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, FTimerDelegate::CreateLambda(
		[this]()
		{ 
			bIsDashing = false;

			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}), 
		DashDuration, false);

}

