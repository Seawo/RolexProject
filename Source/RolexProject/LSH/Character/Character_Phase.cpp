// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Phase.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance_Phase.h"
#include "Animation/AnimMontage.h"
#include "Actor_Effect.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"

#include "AudioComponent_Phase.h"

#include "Actor_Effect_Orb.h"
#include "Actor_Effect_Orb.h"
#include "Actor_Effect_Orb.h"

#include "UI/UI_InGame.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

ACharacter_Phase::ACharacter_Phase()
{
	PrimaryActorTick.bCanEverTick = true;

	Data.RoleType = ERoleType::Dealer;
	Data.Name = "Phase";
	Data.Team = false;
	Data.MaxHp = 250.0f;
	Data.Hp = 250.0f;
	Data.Shield = 0.0f;
	Data.Speed = 400.0f;
	Data.Power = 20.0f;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonPhase/Characters/Heroes/Phase/Meshes/Phase_GDC.Phase_GDC'"));
	if (mesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(mesh.Object);
	}


	AudioComponent = CreateDefaultSubobject<UAudioComponent_Phase>(TEXT("AudioComponent"));

	//EffectMap.Add(TEXT("LMBRMB"), AActor_Effect_Orb::StaticClass());
	//EffectMap.Add(TEXT("E"), AActor_Effect_E::StaticClass());
	//EffectMap.Add(TEXT("Q"), AActor_Effect_Q::StaticClass());
}

void ACharacter_Phase::BeginPlay()
{
	Super::BeginPlay();

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

	//// 화면에 출력하기
	//GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("RMBSkillCoolTime : %.2f"), RMBSkillCoolTime));
	//GEngine->AddOnScreenDebugMessage(0, 0.0f, FColor::Red, FString::Printf(TEXT("ESkillCoolTime : %.2f"), ESkillCoolTime));
	//GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Red, FString::Printf(TEXT("QSkillCoolTime : %.2f"), QSkillCoolTime));


	// 쿨타임 돌리기
	UpdateCoolTime(DeltaTime);



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
		//Jump();
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

void ACharacter_Phase::UpdateCoolTime(float DeltaTime)
{
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

void ACharacter_Phase::SpawnEffect(FName socketName, FName key)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("SpawnEffect"));

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
			

			// FVector를 FRotator로 변환
			TpsCamComp->GetForwardVector();

			// AActor_Effect생성하기
			FRotator rot = TpsCamComp->GetForwardVector().Rotation();
			//AActor_Effect* ef = GetWorld()->SpawnActor<AActor_Effect>(effect, socketLocation, rot);
			AActor_Effect* effect = GetWorld()->SpawnActorDeferred<AActor_Effect>(
									effectClass,
									FTransform(rot, socketLocation),
									this);

			if (effect)
			{
				effect->FinishSpawning(FTransform(rot, socketLocation));
			}
		}
	}
}