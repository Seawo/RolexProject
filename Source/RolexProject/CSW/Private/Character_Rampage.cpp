// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Rampage.h"
#include "EnhancedInputComponent.h"
#include <GameFramework\SpringArmComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "RampageAnimInstance.h"

#include "Animation\AnimInstance.h"
#include "Animation\AnimMontage.h"

#include "EffectActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"

ACharacter_Rampage::ACharacter_Rampage()
{
	PrimaryActorTick.bCanEverTick = true;

	// character setting 
	Data.RoleType = ERoleType::Dealer;
	Data.Name = "Phase";
	Data.Team = true;
	Data.MaxHp = 500;
	Data.Hp = 250;
	Data.Shield = 0;
	Data.Speed = 400.0f;
	Data.Power = 20;

	//SpringArmComp Setting 
	SpringArmComp->SetRelativeLocation(FVector(0, 60, 50));


	// combo_attack
	MaxCnt = 5;
	ComboCnt = 0;
	ComboResetDelay = 1.5f;
	

}

void ACharacter_Rampage::BeginPlay()
{
	Super::BeginPlay();

	// 
	ChangeState(EMoveState::Start, stateMontages[TEXT("Start")]);
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

		// Completed
		characterInput->BindAction(IA_Q, ETriggerEvent::Completed, this, &ACharacter_Rampage::OutputNone);
		characterInput->BindAction(IA_E, ETriggerEvent::Completed, this, &ACharacter_Rampage::OutputNone);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Completed, this, &ACharacter_Rampage::OutputNone);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Completed, this, &ACharacter_Rampage::OutputNone);
	}
}

void ACharacter_Rampage::ChangeAttackState(EAttackState state)
{
	if (IsLocallyControlled())
	{
		Server_ChangeAttackState(state);
	}
}

void ACharacter_Rampage::Server_ChangeAttackState_Implementation(EAttackState attackState)
{
	Multi_ChangeAttackState(attackState);
}

void ACharacter_Rampage::Multi_ChangeAttackState_Implementation(EAttackState attackState)
{
	switch (attackState)
	{
	case EAttackState::QSkill:
		QAttack();
		break;
	case EAttackState::ESkill:
		EAttack();
		break;
	case EAttackState::LMB:
		LBMAttack();
		break;
	case EAttackState::RMB:
		RBMAttack();
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

void ACharacter_Rampage::InputAttack(const FInputActionValue& inputValue)
{
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
		{
			return;
		}
	}

	int inputVector = inputValue.Get<float>();
	inputVector--;
	CurrAttackState = static_cast<EAttackState>(inputVector);
	ChangeAttackState(CurrAttackState);

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
			return; // 공격 애니메이션이 재생 중이라면 더 이상 애니메이션을 받지 않음
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


	PlayAnimMontage(AttackMontages[TEXT("Q")], 0.8f);
	
	CreateStone();
	
}

void ACharacter_Rampage::EAttack()
{
	if (!AttackMontages[TEXT("E")]) return;

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}

	SpringArmComp->SetRelativeLocation(FVector(-200, 60, 70));

	PlayAnimMontage(AttackMontages[TEXT("E")], 1.0f);

	if (bIsDashing)
		return; 

	bIsDashing = true;

	if (IsLocallyControlled())
	{
		Server_DashCheck(bIsDashing);
	}
}

void ACharacter_Rampage::OutputNone(const struct FInputActionValue& inputValue)
{
	int inputVector = inputValue.Get<float>();
	inputVector = 4; // Completed를 위한 아무 숫자 ( 4, 5, 6, 7 중 하나 )
	CurrAttackState = static_cast<EAttackState>(inputVector);
	ChangeAttackState(CurrAttackState);
}

void ACharacter_Rampage::Server_DashCheck_Implementation(bool bIsDash)
{
	Multi_DashCheck(bIsDash);
}

void ACharacter_Rampage::Multi_DashCheck_Implementation(bool bIsDash)
{
	bIsDashing = bIsDash;
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


			SpringArmComp->SetRelativeLocation(FVector(0, 60, 50));

			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}),
		DashDuration, false);
}

void ACharacter_Rampage::CreateStone()
{
	if (!StoneClass) return;

	if (IsLocallyControlled())
	{
		Server_CreateStone();
	}
}

void ACharacter_Rampage::ThrowStone()
{
	if (Stone)
	{
		// 소켓에서 불리
		Stone->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// 던지기
		FVector throwdir = GetActorForwardVector();
		Stone->InititalizeThrowStone(throwdir, 3000.0f);

		SpringArmComp->SetRelativeLocation(FVector(0, 60, 50));
	}
}

void ACharacter_Rampage::Server_CreateStone_Implementation()
{

	USkeletalMeshComponent* skeletalMesh = GetMesh();

	if (!skeletalMesh) return;

	SpringArmComp->SetRelativeLocation(FVector(-200, 60, 70));

	// 소켓 위치 가져오기
	FVector SocketLocation = skeletalMesh->GetSocketLocation(TEXT("hand_r"));
	FRotator SocketRotation = skeletalMesh->GetSocketRotation(TEXT("hand_r"));

	// 돌 액터 붙착
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	Stone = GetWorld()->SpawnActor<AEffectActor>(StoneClass, SocketLocation, SocketRotation, SpawnParams);

	if (Stone)
	{
		// 소켓의 부착
		Stone->AttachToComponent(skeletalMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r"));
	}

}
