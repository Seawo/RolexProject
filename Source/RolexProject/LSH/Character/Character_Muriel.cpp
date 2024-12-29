// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Character/Character_Muriel.h"

#include "GameFramework/CharacterMovementComponent.h"
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

	UpdateCoolTime(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("gravity : %.2f"), GetCharacterMovement()->GravityScale));
	if (bIsPushSpaceBar)
	{
		FlyingTime -= DeltaTime;
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("FlyingTime : %.2f"), FlyingTime));

		if(FlyingTime <= 0)
		{
			bIsPushSpaceBar = false;
			FlyCoolTime = 10.0f;
			GetCharacterMovement()->GravityScale = DefaultGravityScale;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}

	

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("QSkillMovement : %d"), static_cast<int>(QSkillMovement)));
	if (bStartQSkill)
	{
		UpdateQSkillMovement(DeltaTime);
	}
}

void ACharacter_Muriel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter_Muriel::MurielJump);
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Triggered, this, &ACharacter_Muriel::MurielFly);
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Completed, this, &ACharacter_Muriel::StopJumping);
		
		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Muriel::MurielLShift);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielLShift);

		characterInput->BindAction(IA_Q, ETriggerEvent::Triggered, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_Q, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielQSkillComplete);
		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielRMBEnd);
	}
}

void ACharacter_Muriel::ChangeAttackState(EAttackState state)
{



	if (state == EAttackState::QSkill)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QSkill"));
		// 쿨타임
		//if (QSkillCoolTime > 0)
		//{
		//	return;
		//}
		//QSkillCoolTime = 60.0f;

		// LineTrace 쏘기
		FVector start = GetActorLocation();
		FVector end = start + GetActorForwardVector() * 100000.0f;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		FHitResult hitResult;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldStatic, params))
		{
			ABaseCharacter* character = Cast<ABaseCharacter>(hitResult.GetActor());
			if (character and character->Data.Team == Data.Team)
			{
				//UE_LOG(LogTemp, Error, TEXT("My Team"));
				QSkillStartLocation = GetActorLocation();
				QSkillTargetLocation = character->GetActorLocation();
				//UE_LOG(LogTemp, Error, TEXT("QSkillTargetLocation X : %.2f, Y : %.2f, Z : %.2f"), QSkillTargetLocation.X, QSkillTargetLocation.Y, QSkillTargetLocation.Z);
			}
		}

		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.2f, 0, 1.0f);

		// Montage
		//PlayAttackMontage("Q", 1.0f);
		// Effect
	}
	else if (state == EAttackState::ESkill)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ESkill"));
		// 쿨타임
		//if (ESkillCoolTime > 0)
		//{
		//	return;
		//}
		//ESkillCoolTime = 20.0f;
		//// Montage
		//PlayAttackMontage("E", 1.0f);
		//GetCharacterMovement()->GravityScale = 0.7f;
		//Jump();
		// Effect
	}
	else if (state == EAttackState::LMB)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("LMB"));

		// Montage
		PlayAttackMontage("LMB", 1.0f);
		//SpawnEffect("FX_Hand_R4");

		// Effect
	}
	else if (state == EAttackState::RMB)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("RMB"));
		// 쿨타임
		//if (RMBSkillCoolTime > 0)
		//{
		//	return;
		//}
		//RMBSkillCoolTime = 5.0f;

		// Montage
		PlayAttackMontage("RMB", 1.0f);
		// Effect
	}
	else
	{
		return;
	}
}

void ACharacter_Muriel::ChangeState(EMoveState state)
{
}

void ACharacter_Muriel::InputAttack(const FInputActionValue& inputValue)
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

	// RMB공격이 나갈 차례라면
	if (inputVector == 3)
	{
		bIsRMBCharging = true;
	}

	AttackState = static_cast<EAttackState>(inputVector);
	//UE_LOG(LogTemp, Warning, TEXT("AttackState : %d"), static_cast<int>(AttackState));
	ChangeAttackState(AttackState);
}

void ACharacter_Muriel::MurielLShift()
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

void ACharacter_Muriel::MurielFly()
{
	if(not bIsPushSpaceBar) return;

	FVector FlyDirection = GetActorUpVector() * 1.0f;

	// 충돌 감지 후 이동처리
	FHitResult hitResult;
	AddMovementInput(FlyDirection, 1.0f, false); // Move입력 (우선 시도)

	// 앞으로 이동 시 충돌 감지 (벽에 부딪혔는지 확인)
	FVector start = GetActorLocation();
	FVector end = start + FlyDirection * 100.0f;
	FCollisionQueryParams params;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldStatic, params))
	{
		if (hitResult.bBlockingHit)
		{
			// 충돌 시 이동 중단
			AddMovementInput(FlyDirection, 0.0f);
		}
	}
}

void ACharacter_Muriel::MurielJump()
{
	if (bIsPushSpaceBar or FlyCoolTime > 0)
	{
		Jump();
		return;
	}
	//if (FlyCoolTime > 0) return;




	// 중력 초기값 저장
	DefaultGravityScale = GetCharacterMovement()->GravityScale;

	bIsPushSpaceBar = true;
	FlyingTime = 5.0f;
	Jump();

	// 중력 제거
	GetCharacterMovement()->GravityScale = 0.2f;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	// Fly몽타주 재생
	PlayStateMontage("Fly", 1.0f);
}

void ACharacter_Muriel::MurielRMBEnd()
{
	bIsRMBCharging = false;
	// puase인 몽타주 다시 실행
	AnimInstance->Montage_Resume(AttackMontages["RMB"]);
}

void ACharacter_Muriel::MurielQSkillComplete()
{
	//Jump(); // 땅에서 떨어뜨리기 위해
	//SetActorLocation(GetActorLocation() + FVector(0, 0, 100.0f));

	bStartQSkill = true;
	DefaultGravityScale = GetCharacterMovement()->GravityScale;
	QSkillMovement = EQkillMovement::Ascending;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void ACharacter_Muriel::PlayAttackMontage(FString Key, float InPlayRate, FName StartSectionName)
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

void ACharacter_Muriel::PlayStateMontage(FString Key, float InPlayRate, FName StartSectionName)
{
	if (stateMontages.Contains(Key))
	{
		// stateMontages에 있는 몽타주가 실행중이라면 리턴
		for (auto& mon : stateMontages)
		{
			if (AnimInstance->Montage_IsPlaying(mon.Value))
			{
				return;
			}
		}

		UAnimMontage* montage = stateMontages[Key];
		PlayAnimMontage(montage, InPlayRate);
	}
}

void ACharacter_Muriel::UpdateCoolTime(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("FlyCoolTime : %.2f"), FlyCoolTime));
	FlyCoolTime -= DeltaTime;
	if (FlyCoolTime < 0)
	{
		FlyCoolTime = 0;
	}
}

void ACharacter_Muriel::UpdateQSkillMovement(float DeltaTime)
{
	FVector currentLocation = GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("AttackState : %d"), static_cast<int>(AttackState));
	FVector direction;
	
	if (QSkillMovement == EQkillMovement::Ascending)
	{
		GetCharacterMovement()->GravityScale = 0.0f;

		currentLocation.Z += QSkillVerticalSpeed * DeltaTime;

		if (currentLocation.Z >= ZHeight)		//	currentLocation.Z가 ZHeight에 도달하면
		{
			currentLocation.Z = ZHeight;
			QSkillMovement = EQkillMovement::MovingHorizontally;
		}
	}
	else if (QSkillMovement == EQkillMovement::MovingHorizontally)
	{
		FVector horizontalTargetLocation = FVector(QSkillTargetLocation.X, QSkillTargetLocation.Y, ZHeight);
		FVector horizontalDirection = (horizontalTargetLocation - currentLocation).GetSafeNormal2D() * QSkillHorizontalSpeed * DeltaTime;


		currentLocation += horizontalDirection * QSkillHorizontalSpeed * DeltaTime;

		if (FVector::Dist2D(currentLocation, horizontalTargetLocation) <= 10.0f)
		{
			currentLocation = horizontalTargetLocation;
			QSkillMovement = EQkillMovement::Descending;
		}
	}
	else if (QSkillMovement == EQkillMovement::Descending)
	{
		currentLocation.Z -= QSkillVerticalSpeed * DeltaTime;
		if (currentLocation.Z <= QSkillTargetLocation.Z)
		{
			currentLocation.Z = QSkillTargetLocation.Z;
			QSkillMovement = EQkillMovement::Idle;
		}
	}
	else if (QSkillMovement == EQkillMovement::Idle)
	{
		bStartQSkill = false;
		GetCharacterMovement()->GravityScale = DefaultGravityScale;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	SetActorLocation(GetActorLocation() + direction);
}

void ACharacter_Muriel::SpawnEffect(FName socketName, FName key)
{
}
