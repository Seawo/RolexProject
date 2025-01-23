// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Components\CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

#include <GameFramework\SpringArmComponent.h>

#include "HeroUI.h"
#include "RolexGameInstance.h"
#include "RolexPlayerController.h"
#include "Camera\CameraComponent.h"

#include "Animation\AnimMontage.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework\CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = GetCapsuleComponent();
	capsuleComp->SetCapsuleHalfHeight(100.0f);
	capsuleComp->SetCapsuleRadius(50.0f);

	// TPS 카메라 붙이기 
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 40));
	SpringArmComp->TargetArmLength = 160.0f;
	SpringArmComp->SocketOffset = FVector(0.0f,25.0f,75.0f);
	SpringArmComp->bUsePawnControlRotation = true;

	// Camera
	TpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	TpsCamComp->SetupAttachment(SpringArmComp);
	TpsCamComp->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	URolexGameInstance* RolexGameInstance = Cast<URolexGameInstance>(GetGameInstance());
	ARolexPlayerState* RolexPlayerState = Cast<ARolexPlayerState>(GetPlayerState());
	if (RolexGameInstance && RolexPlayerState)
	{
		if (RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID))
			Data.Team = *RolexGameInstance->PlayerTeam.Find(RolexPlayerState->UniqueID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("RolexPlayerState does not exists"));
	}

	auto pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(Imc_base, 0);
		}
	}


	// startPos 저장
	StartPos = GetActorLocation();

	// create widget
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseCharacter::InitHeroUI, 3.0f, false);
}

void ABaseCharacter::InitHeroUI()
{
	if (GetController())
		UE_LOG(LogTemp, Warning, TEXT("Controller Exists"));
	// create UI
	ARolexPlayerController* RolexPlayerController = Cast<ARolexPlayerController>(GetController());
	if (HeroUIFactory && RolexPlayerController && RolexPlayerController->IsLocalPlayerController())
	{
		HeroUI = CreateWidget<UHeroUI>(RolexPlayerController, HeroUIFactory);	// sometimes error occurrs
		if (HeroUI)
		{
			HeroUI->BaseCharacter = this;
			HeroUI->AddToViewport();
		}
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 캐릭터에 hp 달아주기
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 70), FString::Printf(TEXT("HP : %d"), Data.Hp), nullptr, FColor::Green, DeltaTime);
	// 캐릴터에 shield 달아주기
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 80), FString::Printf(TEXT("Shield : %d"), Data.Shield), nullptr, FColor::Green, DeltaTime);

	// 실드 5초간 활성화, 5초 후 비활성화
	//if (bIsShield)
	//{
	//	ShieldTime -= DeltaTime;

	//	if (ShieldTime <= 0.0f)
	//	{
	//		bIsShield = false;
	//	}
	//}
}


void ABaseCharacter::ModifyHP(int Value)
{
	if (!HasAuthority()) return;

	if (Data.Hp <= 0) return;

	// 힐이 들어올 경우
	if (Value > 0)
	{
		Data.Hp += Value;
		if (Data.Hp >= Data.MaxHp)
		{
			Data.Hp = Data.MaxHp;
		}
	}

	// 데미지가 들어올 경우
	else if (Value < 0)
	{
		// 실드가 있는 경우
		if (Data.Shield > 0)
		{
			// 실드가 데미지보다 크거나 같을 경우
			if (Data.Shield + Value >= 0)
			{
				// 실드 차감하기
				Data.Shield += Value;
			}
			// 실드가 데미지보다 작을 경우
			// 실드는 0이 되고 HP에서 나머지 데미지가 차감된다.
			else if (Data.Shield + Value < 0)
			{
				// Value가 음수이기에 +해준다.
				Value += Data.Shield;
				Data.Shield = 0;
				bIsShield = false;
			}
		}

		// 실드가 없으면서 차감될 데미지는 있는 경우
		// 실드가 있지만 들어오는 데미지보다 실드가 작을 경우 차감할 데미지를 실드만큼 차감 시켜주고 HP에게 다시 데미지를 준다.
		if (Data.Shield == 0 and Value < 0) // 실드가 0이 될수도있기때문에 음수로 잡아준다.
		{
			Data.Hp += Value;
			if (Data.Hp <= 0)
			{
				Data.Hp = 0;

				if (stateMontages.Contains("Die"))
				{
					ChangeState(EMoveState::Die, stateMontages["Die"]);
				}
			}
		}
	}
}

void ABaseCharacter::ModifyShield(int shield)
{
	// 실드가 0 이하라면 리턴
	if(shield < 0) return;
	// 비활성화 중일때 리턴
	//if (not bIsShield) return;

	bIsShield = true;
	ShieldTime = 5.0f;
	Data.Shield += shield;
}

FRotator ABaseCharacter::SetAimDirection(ABaseCharacter* character, FVector& targetLocation, FVector startLocation)
{
	if (startLocation == FVector::ZeroVector)
	{
		startLocation = character->GetActorLocation();
	}

	APlayerController* playerController = Cast<APlayerController>(character->GetController());

	FVector camLocation = TpsCamComp->GetComponentLocation();
	FVector camRotation = TpsCamComp->GetForwardVector();
	//playerController->GetPlayerViewPoint(camLocation, camRotation);



	FVector start =  camLocation;
	FVector end = camLocation + camRotation * 10000.0f;

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Pawn, params);
	if (!bHit)
	{
		bHit = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, params);
	}

	//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 1.0f, 0, 1.0f);

	// 발사 방향 설정
	FVector shotDirection = camRotation;
	targetLocation = end;

	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit : %s"), *hitResult.ImpactPoint.ToString());
		targetLocation = hitResult.ImpactPoint;
		DrawDebugPoint(GetWorld(), hitResult.ImpactPoint, 5.0f, FColor::Green, false, 1.0f);
	}
	else
	{
		
		//UE_LOG(LogTemp, Warning, TEXT("Not Hit"));
		//shotDirection = (targetLocation - startLocation).GetSafeNormal();
	}
	shotDirection = (targetLocation - startLocation).GetSafeNormal(); // 목표 방향 보정

	// FVector를 FRotator로 변환
	FRotator rot = shotDirection.Rotation();



	return rot;
}

void ABaseCharacter::OnRep_MoveState()
{
	switch (MoveState)
	{
	case EMoveState::Idle:
		break;
	case EMoveState::Stun:
		if (stateMontages["Stun"])
		{
			Die(stateMontages["Stun"]);
		}
		break;
	case EMoveState::Die:
		if (stateMontages["Die"])
		{
			Die(stateMontages["Die"]);
		}
		break;
	case EMoveState::Start:
		if (stateMontages["Start"])
		{
			Start(stateMontages["Start"]);
		}
		break;
	default:
		break;
	}
}

void ABaseCharacter::OnRep_CharacterData()
{
	// 업데이트 된 hp 확인
	UE_LOG(LogTemp, Warning, TEXT("Hp updated to: %d"), Data.Hp);
}

void ABaseCharacter::MoveDisable()
{
	Sever_MoveDisable();
}

void ABaseCharacter::Sever_MoveDisable_Implementation()
{
	
	GetCharacterMovement()->SetMovementMode(MOVE_None);
}

void ABaseCharacter::MoveEnable()
{
	Sever_MoveEnable();
}

void ABaseCharacter::Sever_MoveEnable_Implementation()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ABaseCharacter::ChangeState(EMoveState newState, UAnimMontage* montage)
{
	Server_ChangeState(newState, montage);
}

void ABaseCharacter::Server_ChangeState_Implementation(EMoveState newState, UAnimMontage* montage)
{
	MoveState = newState;

	switch (newState)
	{
	case EMoveState::Stun:
		Sturn(montage);
		break;
	case EMoveState::Die:
		Die(montage);
		break;
	case EMoveState::Start:
		Start(montage);
		break;
	default:
		break;
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto characterInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (characterInput)
	{
		characterInput->BindAction(IA_VectorMove, ETriggerEvent::Triggered, this, &ABaseCharacter::InputMove);
		characterInput->BindAction(IA_Rotation, ETriggerEvent::Triggered, this, &ABaseCharacter::InputRotation);
	}
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 동기화를 해준다?? 
	DOREPLIFETIME(ABaseCharacter, MoveState);
	DOREPLIFETIME(ABaseCharacter, Data);

}

void ABaseCharacter::StartSkillCool(int32 skillIndex)
{
	// 불가능
	bIsSkillOn[skillIndex] = false;
	GetWorld()->GetTimerManager().SetTimer(SkillCooldownHandles[skillIndex], FTimerDelegate::CreateUObject(
		this,
		&ABaseCharacter::ResetSkillCool,
		skillIndex),
		SkillCooldownDurations[skillIndex],
		false
	);

}

void ABaseCharacter::ResetSkillCool(int32 skillIndex)
{
	// 사용 가능
	bIsSkillOn[skillIndex] = true;

	UE_LOG(LogTemp, Warning, TEXT("Skill %d is ready to use again!"), skillIndex);
}

void ABaseCharacter::InputMove(const FInputActionValue& inputValue)
{
	FVector2D InputVector = inputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRoatation(0, Rotation.Yaw, 0);

		const FVector ForwardDir = FRotationMatrix(YawRoatation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRoatation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, InputVector.Y);
		AddMovementInput(RightDir, InputVector.X);
	}	
}

void ABaseCharacter::InputRotation(const FInputActionValue& inputValue)
{
	FVector2D InputVector = inputValue.Get<FVector2D>();

	if (IsValid(Controller)&&MoveState != EMoveState::Die)
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
		
	}

}

void ABaseCharacter::Sturn(UAnimMontage* montage)
{
	MoveDisable();
	// Test
	PlayAnimMontage(montage);

	float montageDelay = montage->GetPlayLength() + 1.0f;

	FTimerHandle montageTimer;
	GetWorld()->GetTimerManager().SetTimer(montageTimer, FTimerDelegate::CreateLambda(
		[this]() 
		{
			MoveEnable();

		}),
		montageDelay, false);

}

void ABaseCharacter::Die(UAnimMontage* montage)
{
	MoveDisable();
	PlayAnimMontage(montage);

	float montageDelay = montage->GetPlayLength() + 1.0f;
	float DeathTime = 10.0f;
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));


	FTimerHandle montageTimer;
	GetWorld()->GetTimerManager().SetTimer(montageTimer, FTimerDelegate::CreateLambda(
		[this]()
		{
			SetActorLocation(StartPos);
			ChangeState(EMoveState::Start, stateMontages[TEXT("Start")]);

			GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
			MoveEnable();
			Data.Hp = Data.MaxHp;
		}),
		DeathTime, false);
	
}

void ABaseCharacter::Start(UAnimMontage* montage)
{
	//bIsMove = false;
	MoveDisable();
	PlayAnimMontage(montage);

	float montageDelay = montage->GetPlayLength() + 1.0f;

	FTimerHandle montageTimer;
	GetWorld()->GetTimerManager().SetTimer(montageTimer, FTimerDelegate::CreateLambda(
		[this]()
		{
			MoveEnable();
			//bIsMove = true;

		}),
		montageDelay, false);
	
}