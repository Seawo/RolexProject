// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Character_Phase.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance_Phase.h"
#include "Animation/AnimMontage.h"
#include "DataAsset_PhaseLMB.h"
#include "Actor_Effect.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"


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

	AnimInstance = Cast<UAnimInstance_Phase>(GetMesh()->GetAnimInstance());
	
	//PlayMontage("Select", 1.0f);
	//ChangeState(EMoveState::Stun);
}

void ACharacter_Phase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 화면에 출력하기
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("RMBSkillCoolTime : %.2f"), RMBSkillCoolTime));
	GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Red, FString::Printf(TEXT("ESkillCoolTime : %.2f"), ESkillCoolTime));
	GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Red, FString::Printf(TEXT("QSkillCoolTime : %.2f"), QSkillCoolTime));

	// 쿨타임 돌리기
	RMBSkillCoolTime -= DeltaTime;
	if (RMBSkillCoolTime < 0)
	{
		RMBSkillCoolTime = 0.0f;
	}
	ESkillCoolTime -= DeltaTime;
	if (ESkillCoolTime < 0)
	{
		ESkillCoolTime = 0.0f;
	}
	QSkillCoolTime -= DeltaTime;
	if (QSkillCoolTime < 0)
	{
		QSkillCoolTime = 0.0f;
	}
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

void ACharacter_Phase::ChangeAttackState(EAttackState state)
{
	switch (state)
	{
	case EAttackState::QSkill:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QSkill"));
		// 쿨타임
		if (QSkillCoolTime > 0)
		{
			return;
		}
		QSkillCoolTime = 60.0f;
		// Montage
		PlayMontage("Q", 1.0f);
		// Effect
		break;
	case EAttackState::ESkill:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ESkill"));
		// 쿨타임
		if (ESkillCoolTime > 0)
		{
			return;
		}
		ESkillCoolTime = 20.0f;
		// Montage
		PlayMontage("E", 1.0f);
		GetCharacterMovement()->GravityScale = 0.7f;
		Jump();
		// Effect
		break;
	case EAttackState::LMB:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("LMB"));
		// Montage
		PlayMontage("LMB",1.0f);
		//SpawnEffect("FX_Hand_R4");
		
		// Effect
		break;
	case EAttackState::RMB:
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("RMB"));
		// 쿨타임
		if (RMBSkillCoolTime > 0)
		{
			return;
		}
		RMBSkillCoolTime = 5.0f;


		// Montage
		PlayMontage("RMB", 1.0f);
		// Effect
		break;
	default:
		break;
	}
}

void ACharacter_Phase::ChangeState(EMoveState state)
{
	APlayerController* playerController = Cast<APlayerController>(GetController());

	switch (state)
	{
	case EMoveState::Idle:
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		break;
	case EMoveState::Move:
		break;
	case EMoveState::Run:
		break;
	case EMoveState::Jump:
		break;
	case EMoveState::Stun:
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		PlayMontage("Stun", 1.0f);
		break;
	case EMoveState::Die:
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		PlayMontage("Die", 1.0f);
		break;
	case EMoveState::Start:
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		PlayMontage("Select", 1.0f);
		break;
	default:
		break;
	}
}

void ACharacter_Phase::InputAttack(const FInputActionValue& inputValue)
{
	for (auto& mon : AttackMontages)
	{
		if (AnimInstance->Montage_IsPlaying(mon.Value))
		{
			return;
		}
	}


	int inputVector = inputValue.Get<float>();
	inputVector--;
	AttackState = static_cast<EAttackState>(inputVector);
	ChangeAttackState(AttackState);
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
		UAnimMontage* montage = AttackMontages[Key];

		for (auto & mon : AttackMontages)
		{
			if (AnimInstance->Montage_IsPlaying(mon.Value))
			{
				return;
			}
		}
		// 화면에 Key값 출력하기
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Key);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, StartSectionName.ToString());

		PlayAnimMontage(montage, InPlayRate);

		if (Key == "Stun")
		{
			FTimerHandle stunTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, FTimerDelegate::CreateLambda([this]() {MoveState = EMoveState::Idle; ChangeState(MoveState); }), montage->GetPlayLength(), false);
		}

	}
}

void ACharacter_Phase::SpawnEffect(FName socketName, FName key)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("SpawnEffect"));

	if (EffectMap.Contains(key))
	{
		TSubclassOf<AActor_Effect> effect = EffectMap[key];
		if (effect)
		{
			FVector socketLocation;
			if (socketName == "None")
			{
				socketLocation = GetActorLocation();
			}
			else
			{
				// 소켓 위치에 생성시키기
				socketLocation = GetMesh()->GetSocketLocation(socketName);
			}
			

			// FVector를 FRotator로 변환
			TpsCamComp->GetForwardVector();

			// AActor_Effect생성하기
			FRotator rot = TpsCamComp->GetForwardVector().Rotation();
			AActor_Effect* ef = GetWorld()->SpawnActor<AActor_Effect>(effect, socketLocation, rot);
		}
	}
}



//void ACharacter_Phase::SetAimDirection()
//{
//	// 마우스 위치 가져오기
//	APlayerController* playerController = Cast<APlayerController>(GetController());
//
//	if (playerController)
//	{
//		// 플레이어 컨트롤러에서 마우스포이션 가져오기
//		FVector2D mousePosition;
//		playerController->GetMousePosition(mousePosition.X, mousePosition.Y);
//
//		// 스크린 좌표를 월드 좌표로 변환해주기
//		FVector worldLocation, worldDirection;
//		playerController->DeprojectScreenPositionToWorld(mousePosition.X, mousePosition.Y, worldLocation, worldDirection);
//
//		// 라인트레이스 쏘기
//		FHitResult hitResult;
//		GetWorld()->LineTraceSingleByChannel(hitResult, worldLocation, worldLocation + worldDirection * 10000.0f, ECollisionChannel::ECC_Visibility);
//
//		// 라인 그리기
//		DrawDebugLine(GetWorld(), worldLocation, worldLocation + worldDirection * 10000.0f, FColor::Red, false, 0.1f, 0,0.2f);
//
//		// 에임 방향 설정하기
//		AimDirection = hitResult.ImpactPoint - GetActorLocation();
//
//		// Z값을 0으로 만들어주기
//		//AimDirection.Z = 0.0f;
//		AimDirection.Normalize();
//	}
//
//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("AimDirection X : %.2f, Y : %.2f, Z : %.2f"), AimDirection.X, AimDirection.Y, AimDirection.Z));
//}

//void ACharacter_Phase::LMBActionBegin()
//{
//	// 콤보 시작
//	CurrentLMBCombo = 1;
//	FString key = "LMB";
//	if (AttackMontages.Contains(key))
//	{
//		// 화면에 Key값 출력하기
//		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Key);
//		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, StartSectionName.ToString());
//		
//		UAnimMontage* Montage = AttackMontages[key];
//		//PlayAnimMontage(Montage, InPlayRate, StartSectionName);
//
//		// Animation Setting
//		const float attackSpeedRate = 1.0f;
//		//PlayMontage("LMB", attackSpeedRate, "Attack1");
//		
//
//		if (AnimInstance)
//		{
//			AnimInstance->Montage_Play(Montage, attackSpeedRate);
//
//			FOnMontageEnded endDelegate;
//			endDelegate.BindUObject(this, &ACharacter_Phase::LMBActionEnd);
//			AnimInstance->Montage_SetEndDelegate(endDelegate, Montage);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("AnimInstance is nullptr"));
//		}
//
//		LMBTimerHandle.Invalidate();
//		SetLMBCheckTimer(Montage);
//	}
//}
//
//void ACharacter_Phase::LMBActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
//{
//	ensure(CurrentLMBCombo != 0);	// ensur이라는 Assertion 함수를 사용해서 CurrentLMBCombo가 0이 나오면 출력 로그에 에러 발생
//
//	CurrentLMBCombo = 0;
//
//}
//
//void ACharacter_Phase::SetLMBCheckTimer(UAnimMontage* TargetMontage)
//{
//	int32 lmbIndex = CurrentLMBCombo;
//
//	ensure(LMBData->EffectiveFrameCount.IsValidIndex(lmbIndex)); // false면 문제가 있다
//
//	const float attackSpeedRate = 1.0f;
//	// 발동할 시간 정도를 계산
//	float lmbEffectiveTime = (LMBData->EffectiveFrameCount[lmbIndex] / LMBData->FrameRate) / attackSpeedRate;
//	if (lmbEffectiveTime > 0.0f)
//	{
//		GetWorld()->GetTimerManager().SetTimer(LMBTimerHandle, FTimerDelegate::CreateLambda([this, TargetMontage](){LMBCheck(TargetMontage);}), lmbEffectiveTime, false);
//	}
//}
//
//void ACharacter_Phase::LMBCheck(UAnimMontage* TargetMontage)
//{
//	// Handle 초기화
//	LMBTimerHandle.Invalidate();
//	if (bHasNextLMBCommand)
//	{
//		CurrentLMBCombo = FMath::Clamp(CurrentLMBCombo + 1, 1, LMBData->MaxComboCount);
//		FName nextSection = FName(*FString::Printf(TEXT("%s%d"), *LMBData->MontageSectionName, CurrentLMBCombo));
//		AnimInstance->Montage_JumpToSection(nextSection, TargetMontage);
//		SetLMBCheckTimer(TargetMontage);
//		bHasNextLMBCommand = false;
//	}
//}
