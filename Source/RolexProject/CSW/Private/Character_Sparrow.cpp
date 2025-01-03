// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Sparrow.h"
#include "EnhancedInputComponent.h"
#include <GameFramework\SpringArmComponent.h>

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components\SkeletalMeshComponent.h"
#include "SparrowAnimInstance.h"

#include "Camera\CameraComponent.h"
#include "Kismet\GameplayStatics.h"

// 데칼 헤더
#include "Components/DecalComponent.h"
#include "EffectActor.h"
#include "Components/SphereComponent.h"

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

	//AimIndicator = CreateDefaultSubobject<UDecalComponent>(TEXT("AimIndicator"));
	//AimIndicator->SetupAttachment(RootComponent);
	//AimIndicator->SetVisibility(false);
	JumpMaxCount = 2;
}

void ACharacter_Sparrow::BeginPlay()
{
	Super::BeginPlay();

	ChangeState(EMoveState::Start, stateMontages[TEXT("Start")]);


	// 데칼
	AimIndicator = FindComponentByClass<UDecalComponent>();
	AimIndicator->SetVisibility(false);
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
		characterInput->BindAction(IA_RBM, ETriggerEvent::Triggered, this, &ACharacter_Sparrow::AimOffsetRBM);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Completed, this, &ACharacter_Sparrow::ShootingArrowRBM);
	}

}

void ACharacter_Sparrow::ChangeAttackState(EAttackState state)
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

	if (bLBMIsCharging)
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
	if (!bLBMIsCharging) return;


	if (bLBMIsCharging)
	{
		FName sectionName = FName("fire");
		bLBMIsCharging = false;
		PlayAnimMontage(AttackMontages[TEXT("LBM")], 1.0f, *sectionName.ToString());

		SpawnArrow("LBM");

		GetWorld()->GetTimerManager().SetTimer(LBMAimTimerHandle, FTimerDelegate::CreateLambda(
			[this]()
			{
				anim->CameraRot = FRotator(0, 0, 0);
				SpringArmComp->SetRelativeLocation(FVector(0, 10, 40));
				SpringArmComp->TargetArmLength = 160.0f;

			}),
			0.5f, false);
	}
}

void ACharacter_Sparrow::AimOffsetRBM()
{

	if (bRBMIsCharging)
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

void ACharacter_Sparrow::ShootingArrowRBM()
{
	if (!bRBMIsCharging) return;

	if (bRBMIsCharging)
	{
		FName sectionName = FName("fire");
		bRBMIsCharging = false;
		PlayAnimMontage(AttackMontages[TEXT("RBM")], 1.0f, *sectionName.ToString());

		SpawnArrow("RBM");

		GetWorld()->GetTimerManager().SetTimer(RBMAimTimerHandle, FTimerDelegate::CreateLambda(
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

	if (bQIsCharging)
	{
		SpringArmComp->SetRelativeLocation(FVector(0, 0, 500)); // 0,10,40
		SpringArmComp->TargetArmLength = 300; // 원래 160
		
		//TpsCamComp 시점
		//TpsCamComp->SetRelativeLocation(FVector(0, 0, 1500.f));
		TpsCamComp->SetRelativeRotation(FRotator(-45.f, 0, 0));

		// 플레이어 컨트롤러 가져오기
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			// 카메라의 위치와 방향 가져오기
			FVector CameraLocation = TpsCamComp->GetRelativeLocation();
			FRotator CameraRotation = TpsCamComp->GetRelativeRotation();
			PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

			// 카메라 중앙에서 10000 유닛까지 LineTrace
			FVector Start = CameraLocation;
			FVector End = Start + (CameraRotation.Vector() * 10000.0f);

			// LineTrace 수행
			FHitResult HitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);  // 자신은 무시

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
			{

				if (AimIndicator)
				{
					AimIndicator->SetWorldLocation(HitResult.ImpactPoint);
					AimIndicator->SetWorldRotation(HitResult.ImpactNormal.Rotation());

				}

				// 디버그용으로 트레이스를 그려서 확인 (개발 중에만 사용)
				//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);
			}

		}
	}

}

void ACharacter_Sparrow::ShootingArrowQ()
{
	// Q 발사
	// 발싸하는 몽타주 재생
	if (!bQIsCharging) return;

	if (bQIsCharging)
	{
		FName sectionName = FName("fire");
		bQIsCharging = false;
		PlayAnimMontage(AttackMontages[TEXT("Q")], 1.0f, *sectionName.ToString());

		// 데칼 숨기기
		AimIndicator->SetVisibility(false);

		SpawnArrow("Q");

		GetWorld()->GetTimerManager().SetTimer(QAimTimerHandle, FTimerDelegate::CreateLambda(
			[this]()
			{
				SpringArmComp->SetRelativeLocation(FVector(0, 10, 40));
				SpringArmComp->TargetArmLength = 160;

				//TpsCamComp 시점
				TpsCamComp->SetRelativeRotation(FRotator(0, 0, 0));

				// Rain Sparrow

				// 쏘는 곳 위치 알아오기
				FVector TargetLocation = AimIndicator->GetComponentLocation();
	
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				QEffectActor = GetWorld()->SpawnActor<AEffectActor>(QEffectActorclass, TargetLocation, FRotator::ZeroRotator, SpawnParams);
				
				USphereComponent* sphereComp = QEffectActor->FindComponentByClass<USphereComponent>();

				if (sphereComp)
				{
					sphereComp->SetWorldScale3D(FVector(5, 5, 10));
					

					/*
					// 디버그용
					FVector CurrentScale = sphereComp->GetComponentScale();
					float ScaledRadius = sphereComp->GetScaledSphereRadius(); // 스케일 반영된 반지름
					float CapsuleHalfHeight = ScaledRadius * 5.0f;           // 캡슐 길이 (적절히 조정 가능)
					UE_LOG(LogTemp, Log, TEXT("SphereComponent Scale: X=%f, Y=%f, Z=%f"), CurrentScale.X, CurrentScale.Y, CurrentScale.Z);
					UE_LOG(LogTemp, Log, TEXT("Capsule Radius: %f, HalfHeight: %f"), ScaledRadius, CapsuleHalfHeight);

					// DrawDebugCapsule로 길게 표시
					DrawDebugCapsule(
						GetWorld(),
						QEffectActor->GetActorLocation(),       // 캡슐 중심 위치
						CapsuleHalfHeight,                     // 캡슐의 절반 높이
						ScaledRadius,                          // 캡슐 반지름
						FRotationMatrix::MakeFromZ(FVector(0, 0, 1)).ToQuat(),            // 기본 회전값
						FColor::Green,                        // 디버그 색상
						true,                                 // 지속 표시
						5.0f                                  // 지속 시간 (초)
					);
					*/
				}

				// 다시 원래대로 돌아온다
				// 모션을 다 한뒤 마지막 함수 호출로 다시할 예정
				GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			}),
			3.0f, false);
	}
}

void ACharacter_Sparrow::InputJump()
{
	Jump();
}

void ACharacter_Sparrow::InputRun()
{
	if (bQIsCharging || bRBMIsCharging || bLBMIsCharging) return;

	if (!bIsRun)
	{
		bIsRun = true;

		GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
		
	}

}

void ACharacter_Sparrow::CompleteRun()
{
	if (bQIsCharging || bRBMIsCharging || bLBMIsCharging) return;

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

	if (!bLBMIsCharging)
	{
		// 차징중엔 느려지게 하고, 
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;

		FName sectionName = FName("start");
		bLBMIsCharging = true;
		
		PlayAnimMontage(AttackMontages[TEXT("LBM")], 1.0f, *sectionName.ToString());
	}

}

void ACharacter_Sparrow::RBMAttack()
{
	if (!AttackMontages[TEXT("RBM")]) return;

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
		{
			return;
		}
	}

	if (!bRBMIsCharging)
	{
		// 차징중엔 느려지게 하고, 
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;

		FName sectionName = FName("start");
		bRBMIsCharging = true;

		PlayAnimMontage(AttackMontages[TEXT("RBM")], 1.0f, *sectionName.ToString());
	}
}

void ACharacter_Sparrow::QAttack()
{
	if (!AttackMontages[TEXT("Q")]) return;

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
	{
		if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Pair.Value))
			return;
	}

	if (!bQIsCharging)
	{

		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			// 차징중
			bQIsCharging = true;

			// 마우스 커서 작동
			PlayerController->bShowMouseCursor = true;
			PlayerController->SetInputMode(FInputModeGameAndUI());

			// 움직임 막기
			GetCharacterMovement()->DisableMovement();

			FName sectionName = FName("start");
			PlayAnimMontage(AttackMontages[TEXT("Q")], 1.0f, *sectionName.ToString());

			// 데칼 트루
			AimIndicator->SetVisibility(true);
		}
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

void ACharacter_Sparrow::SpawnArrow(FName arrowName)
{
	USkeletalMeshComponent* skeletalMesh = GetMesh();
	
	if (skeletalMesh)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FVector soketPos = skeletalMesh->GetSocketLocation(TEXT("BowEmitterSocket"));
		FRotator soketRot = skeletalMesh->GetSocketRotation(TEXT("BowEmitterSocket"));

		EffectActor = GetWorld()->SpawnActor<AEffectActor>(ArrowClass[arrowName], soketPos, soketRot, SpawnParams);

		if (EffectActor)
		{
			FVector throwdir;
			float speed;

			if (arrowName == "Q")
			{
				// 쏘기
				throwdir = EffectActor->GetActorForwardVector();
				speed = 200.0f;
			}
			else
			{
				// 쏘기
				throwdir = TpsCamComp->GetForwardVector();
				speed = 3000.0f;
			}

			EffectActor->InititalizeThrowStone(throwdir, speed);
		}
	}
}

void ACharacter_Sparrow::SpawnCharge(FName chargeName)
{
	USkeletalMeshComponent* skeletalMesh = GetMesh();

	if (skeletalMesh)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		FVector soketPos = skeletalMesh->GetSocketLocation(TEXT("BowEmitterSocket"));
		FRotator soketRot = skeletalMesh->GetSocketRotation(TEXT("BowEmitterSocket"));

		EffectActor = GetWorld()->SpawnActor<AEffectActor>(ArrowClass[chargeName], soketPos, soketRot, SpawnParams);

	}
}
