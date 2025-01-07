// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Character/Character_Muriel.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


#include "AnimInstance_Muriel.h"
#include "Actor_Effect.h"


ACharacter_Muriel::ACharacter_Muriel()
{
	PrimaryActorTick.bCanEverTick = true;

	Data.RoleType = ERoleType::Healer;
	Data.Name = "Muriel";
	Data.Team = false;
	Data.MaxHp = 200;
	Data.Hp = 200;
	Data.Shield = 0;
	Data.Speed = 400.0f;
	Data.Power = 15;
}

void ACharacter_Muriel::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UAnimInstance_Muriel>(GetMesh()->GetAnimInstance());
}

void ACharacter_Muriel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFlyCoolTime(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("gravity : %.2f"), GetCharacterMovement()->GravityScale));
	if (bIsPushSpaceBar)
	{
		FlyingTime -= DeltaTime;
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("FlyingTime : %.2f"), FlyingTime));

		if(FlyingTime <= 0)
		{
			bIsPushSpaceBar = false;
			FlyCoolTime = 10.0f;
			GetCharacterMovement()->GravityScale = 1.0f;
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}

	if (bIsESkillCharge)
	{
		ESkillSpawnRotation = FRotator(0.0f, SetAimDirection(this, ESkillSpawnLocation).Yaw,0.0f);

		//UE_LOG(LogTemp, Warning, TEXT("ESkillSpawnLocation : %s"), *ESkillSpawnLocation.ToString());
	}

	if (bIsSearchQSkill)
	{
		UpdateQSkillSearchPlayer();
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

		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_Q, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielQSkillComplete);
		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_E, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielESkillComplete);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielRMBEnd);
	}
}

void ACharacter_Muriel::ChangeAttackState(EAttackState state)
{
	if (state == EAttackState::QSkill)
	{
		if (bIsSearchQSkill) return;

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("QSkill"));
		// 쿨타임
		//if (Data.QSkillCoolTime > 0)
		//{
		//	return;
		//}
		//Data.QSkillCoolTime = QSkillRefillTiem;

		bIsSearchQSkill = true;
		

		// Montage
		PlayAttackMontage("Q", 1.0f, "QSkillStart");
		// Effect
	}
	else if (state == EAttackState::ESkill)
	{
		if (bStartESkill) return;

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("ESkill"));
		// 쿨타임
		//if (Data.ESkillCoolTime > 0)
		//{
		//	return;
		//}
		//Data.ESkillCoolTime = ESkillRefillTiem;
		
		bIsESkillCharge = true;

		NearTeamCharacter = nullptr;
		

		// Montage
		PlayAttackMontage("E", 1.0f, "ESkillStart");

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
		if (bIsRMBCharging) return;
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("RMB"));
		// 쿨타임
		//if (Data.RMBCoolTime > 0)
		//{
		//	return;
		//}
		//Data.RMBCoolTime = RMBRefillTiem;
		bIsRMBCharging = true;
		// Montage
		PlayAttackMontage("RMB", 1.0f, "RMBStart");
		// Effect
		SpawnEffect("Muzzle_01", "RMB");
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
	AttackState = static_cast<EAttackState>(inputVector);
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

	//if (FlyCoolTime > 0) return;
	//FlyCoolTime = FlyRefillTiem;

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
	if (not bIsRMBCharging) return;
	bIsRMBCharging = false;
	// puase인 몽타주 다시 실행
	PlayAttackMontage("RMB", 1.0f, "RMBFire");
}

void ACharacter_Muriel::MurielQSkillComplete()
{
	// Q키를 뗐을 때 플레이어를 찾는 행위를 멈추게 하기 위해 bool값 false로 변경
	if (not bIsSearchQSkill) return;
	bIsSearchQSkill = false;


	if (QSkillTargetLocation == FVector::ZeroVector)
	{
		// 목표 지점이 없다면 재생중인 몽타주 멈추기
		AnimInstance->Montage_Stop(0.2f, AttackMontages["Q"]);
		return;
	}

	// 멈춰놨던 몽타주 다시 실행하기
	PlayAttackMontage("Q", 1.0f, "QSkillFire");
	//AnimInstance->Montage_Resume(AttackMontages["Q"]);

	//Jump(); // 땅에서 떨어뜨리기 위해
	//SetActorLocation(GetActorLocation() + FVector(0, 0, 100.0f));

	// QSkill 몽타주를 Tick에서 플레이어의 이동처리 해주기 위한 bool값 true로 변경
	bStartQSkill = true;
	// 중력의 초기값 담아주기
	//DefaultGravityScale = GetCharacterMovement()->GravityScale;
	// QSkill 이동 상태를 시작단계로 변경
	QSkillMovement = EQkillMovement::Ascending;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void ACharacter_Muriel::MurielESkillComplete()
{
	// E스킬 차징중이 아니라면 리턴
	if (not bIsESkillCharge) return;

	bIsESkillCharge = false;

	// Montage 실행
	PlayAttackMontage("E", 1.0f, "ESkillFire");
	SpawnEffect("None", "ESkill");
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
				// QSkill에서 SectionName이 QSkillDescending, QSkillLand, ESkillFire일 때는 다음 몽타주가 실행될수 있도록 리턴하지 않음
				if (StartSectionName == "RMBFire" or
					StartSectionName == "QSkillFire" or
					StartSectionName == "QSkillDescending" or
					StartSectionName == "QSkillLand" or 
					StartSectionName == "ESkillFire")
				{
					continue;
				}
				else
				{
					return;
				}
			}
		}

		UAnimMontage* montage = AttackMontages[Key];
		PlayAnimMontage(montage, InPlayRate, StartSectionName);
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

void ACharacter_Muriel::SpawnEffect(FName socketName, FName key)
{
	if (EffectMap.Contains(key))
	{
		TSubclassOf<AActor_Effect> effectClass = EffectMap[key];
		if (effectClass)
		{
			FVector socketLocation;
			FRotator rot;
			AActor_Effect* effect;


			if (key == "ESkill")
			{
				socketLocation = ESkillSpawnLocation;
				rot = ESkillSpawnRotation;
			}
			else if (key == "QSkill")
			{

			}
			else // LMB RMB
			{
				socketLocation = GetMesh()->GetSocketLocation(socketName);

				if (key == "RMB")
				{
					rot = FRotator::ZeroRotator;
				}
				else if (key == "LMB")
				{
					/** 에임 방향으로 Orb 날리기*/
					FVector target;
					rot = SetAimDirection(this, target, socketLocation);
				}
			}
			

			effect = GetWorld()->SpawnActorDeferred<AActor_Effect>(
				effectClass,
				FTransform(rot, socketLocation),
				this);
			if (effect)
			{
				if (key == "LMB")
				{
					effect->SetIsLMB(true);
				}
				else if (key == "RMB")
				{
					effect->SetIsLMB(false);
				}

				effect->FinishSpawning(FTransform(rot, socketLocation));
			}
		}
	}
}



void ACharacter_Muriel::UpdateFlyCoolTime(float DeltaTime)
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

	if (QSkillMovement == EQkillMovement::Ascending)
	{
		//GetCharacterMovement()->GravityScale = 0.0f;

		currentLocation.Z += QSkillVerticalSpeed * DeltaTime;

		if (currentLocation.Z >= ZHeight)		//	currentLocation.Z가 ZHeight에 도달하면
		{
			currentLocation.Z = ZHeight;
			QSkillMovement = EQkillMovement::MovingHorizontally;
			PlayAttackMontage("Q", 1.0f, "QSkillDescending");
		}
	}
	else if (QSkillMovement == EQkillMovement::MovingHorizontally)
	{
		FVector horizontalTargetLocation = FVector(QSkillTargetLocation.X + 200.0f, QSkillTargetLocation.Y, ZHeight);
		FVector horizontalDirection = (horizontalTargetLocation - currentLocation).GetSafeNormal2D();


		currentLocation += horizontalDirection * QSkillHorizontalSpeed * DeltaTime;

		if (FVector::Dist2D(currentLocation, horizontalTargetLocation) <= 300.0f)
		{
			currentLocation = horizontalTargetLocation;
			QSkillMovement = EQkillMovement::Descending;

		}
	}
	else if (QSkillMovement == EQkillMovement::Descending)
	{
		currentLocation.Z -= QSkillVerticalSpeed * 2.0f * DeltaTime;
		if (currentLocation.Z <= QSkillTargetLocation.Z)
		{
			currentLocation.Z = QSkillTargetLocation.Z;
			currentLocation = FVector(QSkillTargetLocation.X + 200.0f, QSkillTargetLocation.Y, QSkillTargetLocation.Z);


			QSkillMovement = EQkillMovement::Idle;
			PlayAttackMontage("Q", 1.0f, "QSkillLand");
		}
	}
	else if (QSkillMovement == EQkillMovement::Idle)
	{
		bStartQSkill = false;
		GetCharacterMovement()->GravityScale = DefaultGravityScale;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	SetActorLocation(currentLocation);
}

void ACharacter_Muriel::UpdateQSkillSearchPlayer()
{
	QSkillTargetLocation = FVector::ZeroVector;
	//FVector target;
	//SetAimDirection(this, target);

	// LineTrace 쏘기
	// 카메라 rotator값 가져오기
	FVector camRotation = TpsCamComp->GetForwardVector();

	FVector start = TpsCamComp->GetComponentLocation();
	FVector end = start + camRotation * 10000.0f;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	FHitResult hitResult;
	FVector target;

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<FHitResult> hitResults;
	if (GetWorld()->LineTraceMultiByObjectType(hitResults, start, end, FCollisionObjectQueryParams(objectTypes), params))
	{
		for (auto& hit : hitResults)
		{
			ABaseCharacter* character = Cast<ABaseCharacter>(hit.GetActor());
			if (character and character->Data.Team == Data.Team)
			{
				target = hit.ImpactPoint;

				QSkillStartLocation = GetActorLocation();
				QSkillTargetLocation = character->GetActorLocation();
			}
		}
	}
	//if (GetWorld()->LineTraceMultiByChannel(hitResults, start, end, ECollisionChannel::ECC_Pawn, params))
	//{
	//	for (auto& hit : hitResults)
	//	{
	//		ABaseCharacter* character = Cast<ABaseCharacter>(hit.GetActor());
	//		if (character and character->Data.Team == Data.Team)
	//		{
	//			target = hit.ImpactPoint;

	//			QSkillStartLocation = GetActorLocation();
	//			QSkillTargetLocation = character->GetActorLocation();
	//		}
	//	}
	//}


	/*if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Pawn, params))
	{
		ABaseCharacter* character = Cast<ABaseCharacter>(hitResult.GetActor());
		if (character and character->Data.Team == Data.Team)
		{
			target = hitResult.ImpactPoint;

			QSkillStartLocation = GetActorLocation();
			QSkillTargetLocation = character->GetActorLocation();
		}
	}*/

	UE_LOG(LogTemp, Warning, TEXT("QSkillTargetLocation : %s"), *QSkillTargetLocation.ToString());

	DrawDebugPoint(GetWorld(), target, 5.0f, FColor::Green, false, 0.1f);
	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 5.0f, 0, 1.0f);
}