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

	UFUNCTION(Server, Reliable)
	void Server_ChangeAttackState(EAttackState state);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_ChangeAttackState(EAttackState state);

	UFUNCTION(Server, Reliable)
	void Server_SpawnQActor();
public:
	EAttackState CurrentAttackState;

	bool bSkillOngoing = false;
	
	// Q
	UPROPERTY(EditAnywhere)
	int32 NumberOfBalls = 15;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AUltimateBall> UltimateBallFactory;

	// ball min max velocity
	UPROPERTY(EditAnywhere)
	float MinVelocity =1000.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxVelocity =5000.0f;
	
	void QAttack();
	
	// E
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* HealingBox;

	int32 EAttackHealAmount = 10;
	
	void EAttack();

	// LMB
	void LMBAttackStart();
	void StackHeal();
	void  LMBAttack();
	
	UFUNCTION(Server, Reliable)
	void Server_LMBAttack();

	UPROPERTY(EditAnywhere)
	int32 HealValue = 0;
	float HealTime = 0;
	UPROPERTY(EditAnywhere)
	float HealStackRate = 0.25f;
	FTimerHandle StackHealTimer;

	// RMB
	void RMBAttackStart();
	void StackAttack();
	void RMBAttack();

	UFUNCTION(Server, Reliable)
	void Server_RMBAttack();

	// stack value for LMB RMB
	UPROPERTY(EditAnywhere)
	int32 AttackValue = 0;
	float AttackTime = 0;
	UPROPERTY(EditAnywhere)
	float AttackStackRate = 0.25f;
	FTimerHandle StackAttackTimer;

	int32 StackTimeLimit = 5;
	
	bool IsMontagePlaying(UAnimMontage* Montage);
	
public:
 
	
};