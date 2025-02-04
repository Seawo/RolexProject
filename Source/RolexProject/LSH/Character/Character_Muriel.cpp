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
	Data.MaxShield = 500;
	Data.Shield = 0;
	Data.Speed = 400.0f;
	Data.Power = 15;
}

void ACharacter_Muriel::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UAnimInstance_Muriel>(GetMesh()->GetAnimInstance());

	InitHealBarColor();
}

void ACharacter_Muriel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("[Muriel] FlyGauge : %.1f"), FlyGauge));
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Blue, FString::Printf(TEXT("[Muriel] MovementMode : %s"), *GetCharacterMovement()->GetMovementName()));

	//UpdateFlyCoolTime(DeltaTime);
	// 캐릭이 땅에 닿아 있을때만 FlyGauge를 채울 수 있도록 설정
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		FlyGauge += DeltaTime;
		if (FlyGauge > 5.0f)
		{
			FlyGauge = 5.0f;
		}
	}

	if (bIsSearchQSkill)
	{
		UpdateQSkillSearchPlayer();
	}

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
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielFlyComplete);
		
		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Muriel::MurielLShift);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielLShift);

		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_Q, ETriggerEvent::Completed, this, &ACharacter_Muriel::MurielQSkillComplete);

		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Muriel::InputAttack);
		characterInput->BindAction(IA_ERotate, ETriggerEvent::Started, this, &ACharacter_Muriel::MurielESkillRotate);
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
		if (IsLocallyControlled())
		{
			Server_AttackState(state);
		}
		//PlayAttackMontage("Q", 1.0f, "QSkillStart");
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
		if (IsLocallyControlled())
		{
			Server_AttackState(state);
		}
		//PlayAttackMontage("E", 1.0f, "ESkillStart");
	}
	else if (state == EAttackState::LMB)
	{
		// Montage
		if (IsLocallyControlled())
		{
			Server_AttackState(state);
		}
		//PlayAttackMontage("LMB", 1.0f);
	}
	else if (state == EAttackState::RMB)
	{
		if (bIsRMBCharging) return;
		
		bIsRMBCharging = true;
		// Montage
		if (IsLocallyControlled())
		{
			Server_AttackState(state);
		}
		//PlayAttackMontage("RMB", 1.0f, "RMBStart");
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

void ACharacter_Muriel::Server_AttackState_Implementation(EAttackState state)
{
	Multi_AttackState(state);
}

void ACharacter_Muriel::Multi_AttackState_Implementation(EAttackState state)
{
	switch (state)
	{
	case EAttackState::QSkill:
		PlayAttackMontage("Q", 1.0f, "QSkillStart");
		AnnouncerAttackSound("Q");
		break;
	case EAttackState::ESkill:
		bIsESkillCharge = true;
		NearTeamCharacter = nullptr;
		PlayAttackMontage("E", 1.0f, "ESkillStart");
		MurielESkillRotate();
		
		break;
	case EAttackState::LMB:
		PlayAttackMontage("LMB", 1.0f);
		AnnouncerAttackSound("LMB");
		break;
	case EAttackState::RMB:
		bIsRMBCharging = true;
		PlayAttackMontage("RMB", 1.0f, "RMBStart");
		AnnouncerAttackSound("RMB");
		break;
	case EAttackState::QSkill_Completed:
		PlayAttackMontage("Q", 1.0f, "QSkillFire");
		break;
	case EAttackState::ESkill_Completed:
		bIsESkillCharge = false;
		ESkillSpawnRotation = FRotator(0.0f, SetAimDirection(this, ESkillSpawnLocation).Yaw + ESkillRotationYaw, 0.0f);
		PlayAttackMontage("E", 1.0f, "ESkillFire");
		AnnouncerAttackSound("EComplete");
		break;
	case EAttackState::LMB_Completed:
		break;
	case EAttackState::RMB_Completed:
		bIsRMBCharging = false;
		PlayAttackMontage("RMB", 1.0f, "RMBFire");
		break;
	default:
		break;
	}
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
	
	if (bIsSkillOn[inputVector] && MoveState != EMoveState::Die)
	{
		AttackState = static_cast<EAttackState>(inputVector);
		ChangeAttackState(AttackState);
		StartSkillCool(inputVector);
	}
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
	SpacebarHoldTime += GetWorld()->GetDeltaSeconds();
	// 스페이스바를 0.5초 이상 누르고 있지않다면 리턴
	if (SpacebarHoldTime < FlyThreshold)
	{
		return;
	}
	// FlyGauge가 없다면 리턴
	if (FlyGauge <= 0.0f)
	{
		MurielFlyComplete();
		return;
	}
	// 움직이지 못하는 상황일때 (스턴 죽음 Q스킬 등) 리턴시켜주기
	if (GetCharacterMovement()->GetMovementName() == "NULL")
	{
		return;
	}
	// 캐릭터가 바닥에 닿아있다면 리턴 -> 한번 딸깍 눌렀을 때 점프를 사용해주기 위한 코드
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		return;
	}


	// FlyGauge 초당 1씩 차감
	FlyGauge -= GetWorld()->GetDeltaSeconds();

	// Fly중일때 중력값은 0.2f, 이동 모드는 MOVE_Flying으로 변경
	GetCharacterMovement()->GravityScale = 0.2f;
	Server_MoveModeChange(EMovementMode::MOVE_Flying);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);


	// 위방향값 가져오기
	FVector FlyDirection = GetActorUpVector() * 1.0f;
	// 충돌 감지 후 이동처리
	FHitResult hitResult;
	AddMovementInput(FlyDirection, 1.0f, false); // 위 방향으로 이동하기

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
void ACharacter_Muriel::MurielFlyComplete()
{
	if (SpacebarHoldTime < FlyThreshold)
	{
		SpacebarHoldTime = 0.0f;
		StopJumping();
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1.0f;
		Server_MoveModeChange(EMovementMode::MOVE_Falling);
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
}
void ACharacter_Muriel::MurielJump()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		SpacebarHoldTime = 0.0f;
		Jump();
	}
}


void ACharacter_Muriel::MurielRMBEnd()
{
	if (not bIsRMBCharging) return;
	bIsRMBCharging = false;
	// puase인 몽타주 다시 실행
	AttackState = EAttackState::RMB_Completed;
	Server_AttackState(AttackState);
	
	//PlayAttackMontage("RMB", 1.0f, "RMBFire");
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

	// 몽타주 실행하기
	AttackState = EAttackState::QSkill_Completed;
	Server_AttackState(AttackState);
	//PlayAttackMontage("Q", 1.0f, "QSkillFire");

	// QSkill 몽타주를 Tick에서 플레이어의 이동처리 해주기 위한 bool값 true로 변경
	bStartQSkill = true;
	// QSkill 이동 상태를 시작단계로 변경
	QSkillMovement = EQkillMovement::Ascending;
	
}

void ACharacter_Muriel::MurielESkillComplete()
{
	// E스킬 차징중이 아니라면 리턴
	if (not bIsESkillCharge) return;

	bIsESkillCharge = false;

	// Montage 실행
	AttackState = EAttackState::ESkill_Completed;
	Server_AttackState(AttackState);
	//PlayAttackMontage("E", 1.0f, "ESkillFire");

	ESkillSpawnRotation = FRotator(0.0f, SetAimDirection(this, ESkillSpawnLocation).Yaw + ESkillRotationYaw, 0.0f);
	SpawnEffect("None", "ESkill");
}

void ACharacter_Muriel::MurielESkillRotate()
{
	// T키 Input함수
	// E스킬 차징중에 벽의 Rotation을 90도씩 변경시키기 위한 함수
	
	// E스킬 차징중이 아니라면 리턴
	if (not bIsESkillCharge) return;

	bIsESkillRotate = !bIsESkillRotate;

	if (bIsESkillRotate)
	{
		ESkillRotationYaw += 90.0f;
	}
	else
	{
		ESkillRotationYaw -= 90.0f;
	}
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

void ACharacter_Muriel::Server_SpawnEffect_Implementation(FName socketName, FName key)
{
	

	if (EffectMap.Contains(key))
	{
		TSubclassOf<AActor_Effect> effectClass = EffectMap[key];
		if (effectClass)
		{
			FVector socketLocation;
			FRotator rot;
			AActor_Effect* effect;

			// 각 스킬마다의 위치값과 회전값을 설정
			if (key == "ESkill")
			{
				socketLocation = ESkillSpawnLocation;
				rot = ESkillSpawnRotation;
			}
			else if (key == "QSkill")
			{
				socketLocation = GetActorLocation();
				rot = FRotator::ZeroRotator;
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

			// 각 설정된 위치값과 회전값을 바탕으로 이펙스 생성
			effect = GetWorld()->SpawnActorDeferred<AActor_Effect>(
				effectClass,
				FTransform(rot, socketLocation),
				this);
			if (effect)
			{
				// 좌클릭 우클릭일 경우에는 무슨 스킬인지 확인하기 위한 조건문
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

void ACharacter_Muriel::SpawnEffect(FName socketName, FName key)
{
	if (IsLocallyControlled())
	{
		Server_SpawnEffect(socketName, key);
	}
}

void ACharacter_Muriel::Server_MoveModeChange_Implementation(EMovementMode moveMode)
{
	
	GetCharacterMovement()->SetMovementMode(moveMode);
}

void ACharacter_Muriel::Server_MoveToLocation_Implementation(FVector newLocation)
{
	SetActorLocation(newLocation);
}

void ACharacter_Muriel::UpdateQSkillMovement(float DeltaTime)
{
	FVector currentLocation = GetActorLocation();

	// 수직 상승 중일때
	if (QSkillMovement == EQkillMovement::Ascending)
	{
		//currentLocation.Z += QSkillVerticalSpeed * DeltaTime;
		currentLocation.Z += QSkillVerticalSpeed * GetWorld()->GetDeltaSeconds();



		Server_MoveModeChange(EMovementMode::MOVE_None);
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		
		if (currentLocation.Z >= ZHeight)		//	수직 상승 중에 수평이동 시작할 위치에 도착했다면
		{
			

			currentLocation.Z = ZHeight;
			QSkillMovement = EQkillMovement::MovingHorizontally;

			PlayAttackMontage("Q", 1.0f, "QSkillDescending");

		}
	}
	// 수평 이동 중일때
	else if (QSkillMovement == EQkillMovement::MovingHorizontally)
	{
		FVector horizontalTargetLocation = FVector(QSkillTargetLocation.X + 200.0f, QSkillTargetLocation.Y, ZHeight);
		FVector horizontalDirection = (horizontalTargetLocation - currentLocation).GetSafeNormal2D();


		currentLocation += horizontalDirection * QSkillHorizontalSpeed * DeltaTime;
		Server_MoveModeChange(EMovementMode::MOVE_None);
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);


		if (FVector::Dist2D(currentLocation, horizontalTargetLocation) <= 300.0f) // 수평 이동중에 수직 하강 위치에 도착했다면
		{
			currentLocation = horizontalTargetLocation;
			QSkillMovement = EQkillMovement::Descending;

		}
	}
	// 수직 하강 중일때
	else if (QSkillMovement == EQkillMovement::Descending)
	{

		currentLocation.Z -= QSkillVerticalSpeed * 2.0f * DeltaTime;
		Server_MoveModeChange(EMovementMode::MOVE_None);
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		if (currentLocation.Z <= QSkillTargetLocation.Z) // 착지지점에 도착했다면
		{
			SpawnEffect("None", "QSkill");


			currentLocation.Z = QSkillTargetLocation.Z;
			currentLocation = FVector(QSkillTargetLocation.X + 200.0f, QSkillTargetLocation.Y, QSkillTargetLocation.Z);


			QSkillMovement = EQkillMovement::Idle;
			PlayAttackMontage("Q", 1.0f, "QSkillLand");
			//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
	// 착지 상태일때
	else if (QSkillMovement == EQkillMovement::Idle)
	{
		bStartQSkill = false;
		GetCharacterMovement()->GravityScale = DefaultGravityScale;
		Server_MoveModeChange(EMovementMode::MOVE_Walking);
		//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	Server_MoveToLocation(currentLocation);
	//SetActorLocation(currentLocation);
}

void ACharacter_Muriel::UpdateQSkillSearchPlayer()
{
	QSkillTargetLocation = FVector::ZeroVector;

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

	//UE_LOG(LogTemp, Warning, TEXT("QSkillTargetLocation : %s"), *QSkillTargetLocation.ToString());

	DrawDebugPoint(GetWorld(), target, 5.0f, FColor::Green, false, 0.1f);
	//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 5.0f, 0, 1.0f);
}