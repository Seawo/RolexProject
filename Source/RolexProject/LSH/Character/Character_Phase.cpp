// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Phase.h"

#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "AnimInstance_Phase.h"
#include "Animation/AnimMontage.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "AudioComponent_Phase.h"

#include "Actor_Effect.h"
#include "Actor_Effect_Phase_Orb.h"

#include "UI/UI_InGame.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Net/UnrealNetwork.h"

#include "EngineUtils.h"

ACharacter_Phase::ACharacter_Phase()
{
	PrimaryActorTick.bCanEverTick = true;

	Data.RoleType = ERoleType::Dealer;
	Data.Name = "Phase";
	Data.Team = false;
	Data.MaxHp = 250;
	Data.Hp = 200;
	Data.Shield = 0;
	Data.Speed = 400.0f;
	Data.Power = 20;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
	}


	AudioComponent = CreateDefaultSubobject<UAudioComponent_Phase>(TEXT("AudioComponent"));
}

void ACharacter_Phase::BeginPlay()
{
	Super::BeginPlay();
	/*Data.Hp = 200.0f;*/
	AnimInstance = Cast<UAnimInstance_Phase>(GetMesh()->GetAnimInstance());
	
	if (UI_InGameClass)
	{
		UI_InGame = CreateWidget<UUI_InGame>(GetWorld(), UI_InGameClass);
		if (UI_InGame)
		{
			UI_InGame->AddToViewport();
		}
	
	}
	//PlayMontage("Select", 1.0f);
	//ChangeState(EMoveState::Stun);
}

void ACharacter_Phase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 화면에 출력하기
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("RMBSkillCoolTime : %.2f"), Data.RMBCoolTime));
	GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Red, FString::Printf(TEXT("ESkillCoolTime : %.2f"), Data.ESkillCoolTime));
	GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Red, FString::Printf(TEXT("QSkillCoolTime : %.2f"), Data.QSkillCoolTime));


	// 쿨타임 돌리기
	//UpdateCoolTime(DeltaTime);



	// 임시로 UI 출력
	if (UI_InGame)
	{
		UpdateUI();
	}
}

void ACharacter_Phase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter_Phase::PhaseJump);
		characterInput->BindAction(IA_Spacebar, ETriggerEvent::Completed, this, &ACharacter_Phase::StopJumping);

		characterInput->BindAction(IA_LShift, ETriggerEvent::Started, this, &ACharacter_Phase::InputLShift);
		characterInput->BindAction(IA_LShift, ETriggerEvent::Completed, this, &ACharacter_Phase::InputLShift);

		characterInput->BindAction(IA_Q, ETriggerEvent::Started, this, &ACharacter_Phase::InputAttack);
		characterInput->BindAction(IA_E, ETriggerEvent::Started, this, &ACharacter_Phase::InputAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Started, this, &ACharacter_Phase::InputAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Started, this, &ACharacter_Phase::InputAttack);

		// completed
		characterInput->BindAction(IA_Q, ETriggerEvent::Completed, this, &ACharacter_Phase::OutPutAttack);
		characterInput->BindAction(IA_E, ETriggerEvent::Completed, this, &ACharacter_Phase::OutPutAttack);
		characterInput->BindAction(IA_LBM, ETriggerEvent::Completed, this, &ACharacter_Phase::OutPutAttack);
		characterInput->BindAction(IA_RBM, ETriggerEvent::Completed, this, &ACharacter_Phase::OutPutAttack);

	}
}

void ACharacter_Phase::ChangeAttackState(EAttackState state)
{	
	if (IsLocallyControlled())
	{
		Server_ChangeAttackState(AttackState);
	}
}

void ACharacter_Phase::Server_ChangeAttackState_Implementation( EAttackState attackState)
{
	Multi_ChangeAttackState(attackState);
}

void ACharacter_Phase::Multi_ChangeAttackState_Implementation( EAttackState attackState)
{
	//UE_LOG(LogTemp, Warning, TEXT("GetUniqueID : %d "), GetUniqueID());
	//UE_LOG(LogTemp, Warning, TEXT("OtherID : %d "), id);

	//UE_LOG(LogTemp, Warning, TEXT("Multi_ChangeAttackState called for AttackState: %d"), (int32)AttackState);

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

/** 플레이어 스킬 및 공격 함수들*/


/** 플레이어 Input함수들*/
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
void ACharacter_Phase::OutPutAttack(const struct FInputActionValue& inputValue)
{
	for (auto& mon : AttackMontages)
	{
		if (AnimInstance->Montage_IsPlaying(mon.Value))
		{
			return;
		}
	}

	int inputVector = inputValue.Get<float>();
	inputVector = 4; // Completed를 위한 아무 숫자 ( 4, 5, 6, 7 중 하나 )
	AttackState = static_cast<EAttackState>(inputVector);
	ChangeAttackState(AttackState);

}
void ACharacter_Phase::PhaseJump()
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		AudioComponent->PlaySound("Jump");
	}
	Jump();
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


/** 플레이어 공격 함수들*/
void ACharacter_Phase::LBMAttack()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("LMB"));

	// Montage
	PlayMontage("LMB", 1.0f);
}
void ACharacter_Phase::RBMAttack()
{
	// 쿨타임
	if (Data.RMBCoolTime > 0)
	{
		return;
	}
	Data.RMBCoolTime = RMBRefillTime;


	// Montage
	PlayMontage("RMB", 1.0f);
}
void ACharacter_Phase::EAttack()
{
	// 쿨타임
	if (Data.ESkillCoolTime > 0)
	{
		return;
	}
	Data.ESkillCoolTime = ESkillRefillTime;

	// Montage
	PlayMontage("E", 1.0f);
	GetCharacterMovement()->GravityScale = 0.7f;
}
void ACharacter_Phase::QAttack()
{
	// 쿨타임
	if (Data.QSkillCoolTime > 0)
	{
		return;
	}
	Data.QSkillCoolTime = QSkillRefillTime;

	// Montage
	PlayMontage("Q", 1.0f);
}


/** 몽타주 재생 및 스킬 액터 스폰 함수들*/
void ACharacter_Phase::PlayMontage(FString Key, float InPlayRate, FName StartSectionName)
{
	if (AttackMontages.Contains(Key))
	{
		for (auto & mon : AttackMontages)
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

void ACharacter_Phase::SpawnEffect(FName socketName, FName key)
{
	if (IsLocallyControlled())
	{
		Server_SpawnEffect(socketName, key);
	}
}


void ACharacter_Phase::Server_SpawnEffect_Implementation(FName socketName, FName key)
{

	if (EffectMap.Contains(key))
	{
		TSubclassOf<AActor_Effect> effectClass = EffectMap[key];
		if (effectClass)
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


			/** 에임 방향으로 Orb 날리기*/
			FVector target;
			FRotator rot = SetAimDirection(this, target, socketLocation);

			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			AActor_Effect* effect = GetWorld()->SpawnActor<AActor_Effect>(effectClass, socketLocation, rot, spawnParams);
		}
	}
}




/**Tick 함수에서 계속 돌아가야할 함수들*/
void ACharacter_Phase::UpdateUI()
{
	UI_InGame->PB_HPBar->SetPercent(Data.Hp / Data.MaxHp);
	int rmb = RMBSkillCoolTime;
	int e = ESkillCoolTime;
	int q = QSkillCoolTime;

	UI_InGame->Text_RMBCooltime->SetText(FText::AsNumber(rmb));
	UI_InGame->Text_ESkillCooltime->SetText(FText::AsNumber(e));
	UI_InGame->Text_QSkillCooltime->SetText(FText::AsNumber(q));
}