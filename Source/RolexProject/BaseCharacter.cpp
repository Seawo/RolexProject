// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Components\CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"

#include <GameFramework\SpringArmComponent.h>
#include "Camera\CameraComponent.h"

#include "Animation\AnimMontage.h"
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
	
	auto pc = Cast<APlayerController>(Controller);

	if (pc)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(Imc_base, 0);
		}
	}

}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 캐릭터에 hp 달아주기
	DrawDebugString(GetWorld(), GetActorLocation() + FVector(0, 0, 70), FString::Printf(TEXT("HP : %d"), Data.Hp), nullptr, FColor::Green, DeltaTime);
}


void ABaseCharacter::ModifyHP(int Value)
{
	if (Data.Hp <= 0) return;

	Data.Hp += Value;
	// Value > 0 : 힐, Value < 0 : 데미지
	if (Data.Hp >= Data.MaxHp)
	{
		Data.Hp = Data.MaxHp;
	}
	else if (Data.Hp <= 0)
	{
		Data.Hp = 0;
		if (stateMontages.Contains("Die"))
		{
			ChangeState(EMoveState::Die, stateMontages["Die"]);
		}
	}
}

void ABaseCharacter::ModifyShield(int shield)
{
	if(shield < 0) return;

	Data.Shield += shield;
}

FRotator ABaseCharacter::SetAimDirection(ABaseCharacter* character, FVector& targetLocation, FVector startLocation)
{
	if (startLocation == FVector::ZeroVector)
	{
		startLocation = character->GetActorLocation();
	}

	APlayerController* playerController = Cast<APlayerController>(character->GetController());

	FVector camLocation;
	FRotator camRotation;
	playerController->GetPlayerViewPoint(camLocation, camRotation);

	FVector start = camLocation;
	FVector end = camLocation + camRotation.Vector() * 10000.0f;

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
	FVector shotDirection = camRotation.Vector();
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

void ABaseCharacter::ChangeState(EMoveState newState, UAnimMontage* montage)
{
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

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}

}

void ABaseCharacter::Sturn(UAnimMontage* montage)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// Test
	PlayAnimMontage(montage);

	float montageDelay = montage->GetPlayLength() + 1.0f;

	FTimerHandle montageTimer;
	GetWorld()->GetTimerManager().SetTimer(montageTimer, FTimerDelegate::CreateLambda(
		[this]() 
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		}),
		montageDelay, false);

}

void ABaseCharacter::Die(UAnimMontage* montage)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayAnimMontage(montage);

	float montageDelay = montage->GetPlayLength() + 1.0f;

	FTimerHandle montageTimer;
	GetWorld()->GetTimerManager().SetTimer(montageTimer, FTimerDelegate::CreateLambda(
		[this]()
		{
			
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		}),
		montageDelay, false);
	
}

void ABaseCharacter::Start(UAnimMontage* montage)
{
	//bIsMove = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayAnimMontage(montage);

	float montageDelay = montage->GetPlayLength() + 1.0f;

	FTimerHandle montageTimer;
	GetWorld()->GetTimerManager().SetTimer(montageTimer, FTimerDelegate::CreateLambda(
		[this]()
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			//bIsMove = true;

		}),
		montageDelay, false);
	
}

