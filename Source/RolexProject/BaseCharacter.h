// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle,
	Move,
	Run,
	Jump
};

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	QSkill,
	ESkill,
	LMB,
	RMB
};

UENUM(BlueprintType)
enum class ERoleType : uint8
{
	Tanker,
	Dealer,
	Healer
};

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_BODY()

public:
	ERoleType RoleType;		// 역할

	FString Name;			// 캐릭터 이름
	bool Team;				// 팀 여부 (true = Red , false = Blue)

	// Stat
	float MaxHp;			// 최대 체력
	float Hp;				// 현재 체력
	float Shield;			// 쉴드
	float Speed;			// 이동속도
	float Power;			// 공격력
};

UCLASS()
class ROLEXPROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ChangeState(EAttackState newState) PURE_VIRTUAL(ABaseCharacter::ChangeState,);
	virtual void InputAttack(const struct FInputActionValue& inputValue) PURE_VIRTUAL(ABaseCharacter::InputAttack,);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	FCharacterData Data;				// 캐릭터 데이터를 가지고있을 구조체
	EMoveState MoveState;				// 이동 상태
	EAttackState AttackState;			// 공격 상태

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	TMap<FString, UAnimMontage*> AttackMontages;

/*Input*/
public:
	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* capsuleComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* TpsCamComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* Imc_base;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_VectorMove;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Rotation;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_LShift;						// 달리기
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Spacebar;					// 점프
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_LBM;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_RBM;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_Q;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* IA_E;


	void InputMove(const struct FInputActionValue& inputValue);
	void InputRotation(const struct FInputActionValue& inputValue);
};
