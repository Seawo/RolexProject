// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Fey.generated.h"

UCLASS()
class ROLEXPROJECT_API AFey : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFey();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void InputAttack(const struct FInputActionValue& inputValue) override;
	
	virtual void ChangeAttackState(EAttackState newState) override;

public:
	EAttackState CurrentAttackState;

	UPROPERTY(EditAnywhere)
	int32 NumberOfBalls = 15;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AUltimateBall> UltimateBallFactory;
	
	UPROPERTY(EditAnywhere)
	float SpreadAngle = 45.0f;
	
	void QAttack();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* HealingBox;

	int32 EAttackHealAmount = 10;
	
	void EAttack();
	
	void LMBAttackStart();
	void StackHeal();
	void  LMBAttack();

	UPROPERTY(EditAnywhere)
	int32 HealValue = 0;
	int32 HealTime = 0;
	FTimerHandle StackHealTimer;
	
	void RMBAttackStart();
	void StackAttack();
	void RMBAttack();

	UPROPERTY(EditAnywhere)
	int32 AttackValue = 0;
	int32 AttackTime = 0;
	FTimerHandle StackAttackTimer;

	int32 StackTimeLimit = 5;
	
	bool IsMontagePlaying(UAnimMontage* Montage);
	
public:
 
	
};