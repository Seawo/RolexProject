// Fill out your copyright notice in the Description page of Project Settings.


#include "Fey.h"

#include "EnhancedInputComponent.h"
#include "UltimateBall.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFey::AFey()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Data.RoleType = ERoleType::Healer;
	Data.Name = "Fey";
	Data.Team = true;
	Data.MaxHp = 250.0f;
	Data.Hp = Data.MaxHp;
	Data.Shield = 0.0f;
	Data.Speed = 400.0f;
	Data.Power = 10.0f;

	// healing Box
	HealingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HealingBox"));
	HealingBox->SetupAttachment(GetRootComponent());
	
	HealingBox->SetBoxExtent(FVector(1000.0f, 500.0f, 500.0f));
	HealingBox->SetRelativeLocation(FVector(1000.0f, 0.0f, 0.0f));

	// collision settings: ignore all but overlap pawn
	HealingBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HealingBox->SetCollisionObjectType(ECC_WorldDynamic);
	HealingBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HealingBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	HealingBox->SetHiddenInGame(false);
	HealingBox->SetVisibility(true);
}

// Called when the game starts or when spawned
void AFey::BeginPlay()
{
	Super::BeginPlay();

	SpringArmComp->SetRelativeRotation(FRotator(0, 60, 50));
	ChangeState(EMoveState::Start, stateMontages[TEXT("Start")]);
}

// Called every frame
void AFey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFey::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Spacebar, ETriggerEvent::Started, this, &ACharacter::Jump);
		
		EnhancedInputComponent->BindAction(IA_Q, ETriggerEvent::Started, this, &AFey::InputAttack);
		EnhancedInputComponent->BindAction(IA_E, ETriggerEvent::Started, this, &AFey::InputAttack);
		EnhancedInputComponent->BindAction(IA_LBM, ETriggerEvent::Started, this, &AFey::InputAttack);
		EnhancedInputComponent->BindAction(IA_LBM, ETriggerEvent::Completed, this, &AFey::LMBAttack);
		EnhancedInputComponent->BindAction(IA_RBM, ETriggerEvent::Started, this, &AFey::InputAttack);
		EnhancedInputComponent->BindAction(IA_RBM, ETriggerEvent::Completed, this, &AFey::RMBAttack);
	}
}

void AFey::InputAttack(const FInputActionValue& inputValue)
{
	int32 InputVector = inputValue.Get<float>();	// each input value are represented as a float val
	InputVector--;
	CurrentAttackState = static_cast<EAttackState>(InputVector);
	ChangeAttackState(CurrentAttackState);
}

void AFey::ChangeAttackState(EAttackState newState)
{
	switch (newState)
	{
	case EAttackState::QSkill:
		QAttack();
		break;
	case EAttackState::ESkill:
		EAttack();
		break;
	case EAttackState::LMB:
		LMBAttackStart();
		break;
	case EAttackState::RMB:
		RMBAttackStart();
		break;
	default:
		break;
	}
}

void AFey::QAttack()
{
	 FString AttackName = TEXT("Q");
	if (AttackMontages[AttackName] == nullptr) return;

	// block montage being overlapped
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
		if (IsMontagePlaying(Pair.Value)) return;

	// spawn heal & damage balls
	for (int32 i=0; i<NumberOfBalls; i++)
	{
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector()*100.0f;
		FRotator SpawnRotation = FRotator(0.0f, FMath::FRandRange(-SpreadAngle, SpreadAngle), 0.0f);

		AUltimateBall* Ball = GetWorld()->SpawnActor<AUltimateBall>(UltimateBallFactory, SpawnLocation, SpawnRotation);
		if (Ball)
		{
			// TODO: study projectilemovement and increase Z velocity, enable floor collision
			Ball->ProjectileMovement->SetVelocityInLocalSpace(FVector(1000.0f, 0.0f, 500.0f));
			Ball->ProjectileMovement->Activate();
		}
	}
	
	SpringArmComp->SetRelativeLocation(FVector(-200, 60, 70));
	PlayAnimMontage(AttackMontages[AttackName], 1.0f);
	SpringArmComp->SetRelativeLocation(FVector(0, 60, 50));
}

void AFey::EAttack()
{
	FString AttackName = TEXT("E");
	if (AttackMontages[AttackName] == nullptr) return;

	// block montage being overlapped
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
		if (IsMontagePlaying(Pair.Value)) return;	

	// // debug
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	// {
	// 	HealingBox->SetVisibility(true);
	// 	HealingBox->ShapeColor = FColor::Green;
	// },
	// 	1.0f,
	// 	false
	// 	);
	
	// heal characters within 100 units in front of Fey
	TArray<AActor*> OverlappingActors;
	HealingBox->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(Actor);
		if (TargetCharacter && TargetCharacter->Data.Team == Data.Team)
		{
			if (TargetCharacter == this) continue;
			UE_LOG(LogTemp, Warning, TEXT("TargetCharacter Name: %s"), *TargetCharacter->GetName());
			TargetCharacter->Data.Hp += EAttackHealAmount;
			if (TargetCharacter->Data.Hp > TargetCharacter->Data.MaxHp)
				TargetCharacter->Data.Hp = TargetCharacter->Data.MaxHp;
		}
	}
	
	SpringArmComp->SetRelativeLocation(FVector(-200, 60, 70));
	PlayAnimMontage(AttackMontages[AttackName], 1.0f);
	SpringArmComp->SetRelativeLocation(FVector(0, 60, 50));
}

void AFey::LMBAttackStart()
{
	HealTime = 0;
	HealValue = 0;
	
	GetWorld()->GetTimerManager().SetTimer(StackHealTimer, this, &AFey::StackHeal, 1.0f, true);
}

void AFey::StackHeal()
{
	HealTime += 1;
	HealValue += 5;

	UE_LOG(LogTemp, Warning, TEXT("Heal Stacking"));
	
	if (HealTime > StackTimeLimit)
		LMBAttack();
}

void AFey::LMBAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(StackHealTimer);

	// check montage is available
	FString AttackName = TEXT("LMB");
	if (AttackMontages[AttackName] == nullptr) return;

	// block montage being overlapped
	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
		if (IsMontagePlaying(Pair.Value)) return;	

	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector()*1000.0f;
	float Radius = 50.0f;
	float HalfHeight = 100.0f;
	
	// TODO: make float values as a variable
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeCapsule(Radius, HalfHeight)
		);

	// collision detection region
	DrawDebugCapsule(
		GetWorld(),
		(Start + End)*0.5f,
		HalfHeight,
		Radius,
		FQuat::Identity,
		bHit ? FColor::Red : FColor::Green,
		false,
		2.0f
		);

	if (bHit)
		UE_LOG(LogTemp, Warning, TEXT("Heal Value: %d"), HealValue);
	
	SpringArmComp->SetRelativeLocation(FVector(-200, 60, 70));
	PlayAnimMontage(AttackMontages[AttackName], 1.0f);
	SpringArmComp->SetRelativeLocation(FVector(0, 60, 50));
}

void AFey::RMBAttackStart()
{
	AttackTime = 0;
	AttackValue = 0;
	
	GetWorld()->GetTimerManager().SetTimer(StackAttackTimer, this, &AFey::StackAttack, 1.0f, true);
}

void AFey::StackAttack()
{
	AttackTime += 1;
	AttackValue += 5;

	if (AttackTime > StackTimeLimit)
		RMBAttack();
}

void AFey::RMBAttack()
{
	FString AttackName = TEXT("RMB");
	if (AttackMontages[AttackName] == nullptr) return;

	for (const TPair<FString, UAnimMontage*>& Pair : AttackMontages)
		if (IsMontagePlaying(Pair.Value)) return;	

	SpringArmComp->SetRelativeLocation(FVector(-200, 60, 70));
	PlayAnimMontage(AttackMontages[AttackName], 1.0f);
	SpringArmComp->SetRelativeLocation(FVector(0, 60, 50));
}

bool AFey::IsMontagePlaying(UAnimMontage* Montage)
{
	if (GetMesh()->GetAnimInstance()->Montage_IsPlaying(Montage)) return true;
	else return false;
}
