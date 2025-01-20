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
	Jump,
	Stun,
	Die,
	Start,
};

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	QSkill,
	ESkill,
	LMB,
	RMB,
	QSkill_Completed,
	ESkill_Completed,
	LMB_Completed,
	RMB_Completed
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
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ERoleType RoleType;		// 역할
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;			// 캐릭터 이름
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Team;				// 팀 여부 (true = Red , false = Blue)

	// Stat
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxHp;				// 최대 체력
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Hp;					// 현재 체력
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Shield = -1;		// 쉴드
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed;			// 이동속도
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Power;				// 공격력

	// CoolTime
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RMBCoolTime = 0.0f;		// 우클릭 쿨타임
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ESkillCoolTime = 0.0f;	// E스킬 쿨타임
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float QSkillCoolTime = 0.0f;	// Q스킬 쿨타임
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

	virtual void ChangeAttackState(EAttackState newState) PURE_VIRTUAL(ABaseCharacter::ChangeState,);
	virtual void InputAttack(const struct FInputActionValue& inputValue) PURE_VIRTUAL(ABaseCharacter::InputAttack,);

	void ChangeState(EMoveState newState, UAnimMontage* montage);
	void UpdateCoolTime(float DeltaTime);		// 쿨타임 업데이트 함수(Tick)


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_CharacterData)
	FCharacterData Data;				// 캐릭터 데이터를 가지고있을 구조체
	
	UPROPERTY(ReplicatedUsing = OnRep_MoveState)
	EMoveState MoveState;				// 이동 상태
	EAttackState AttackState;			// 공격 상태

	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<FString, UAnimMontage*> AttackMontages;
	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<FString, UAnimMontage*> stateMontages;

	bool bIsShield = false;
	float ShieldTime = 5.0f;



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

	void Sturn(UAnimMontage* montage);
	void Die(UAnimMontage* montage);
	void Start(UAnimMontage* montage);

	void ModifyHP(int Value);
	void ModifyShield(int shield);

	FRotator SetAimDirection(ABaseCharacter* character, FVector& targetLocation, FVector startLocation = FVector::ZeroVector);


	UFUNCTION()
	void OnRep_MoveState();

	UFUNCTION()
	void OnRep_CharacterData();

	// 움직임 막기
	void MoveDisable();
	UFUNCTION(Server, Reliable)
	void Sever_MoveDisable();
	// 다시 움직이기
	void MoveEnable();
	UFUNCTION(Server, Reliable)
	void Sever_MoveEnable();


	// 카메라 동기화 
	FVector StartPos;

public:
	// hero UI image
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UI")
	class UTexture2D* BaseCharacterImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UI")
	UTexture2D* SkillImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UI")
	UTexture2D* WeaponImage;

	// create UI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="UI")
	TSubclassOf<class UHeroUI> HeroUIFactory;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	UHeroUI* HeroUI;

	void InitHeroUI();
};
