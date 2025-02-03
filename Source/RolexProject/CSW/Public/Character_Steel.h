// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Character_Steel.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API ACharacter_Steel : public ABaseCharacter
{
	GENERATED_BODY()

	ACharacter_Steel();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ChangeAttackState(EAttackState state) override;
	//virtual void ChangeState(EMoveState state) override;
	virtual void InputAttack(const struct FInputActionValue& inputValue) override;

public:
	EAttackState CurrAttackState;

	// LMB 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<class AEffectActor> LMBEffectActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* LMBParticleEffect;

	class AEffectActor* LMBEffectActor;

	// RMB
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEffectActor> ShieldClass;

	class AEffectActor* Shield;

	// E
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<class AEffectActor> EEffectActorClass;

	class AEffectActor* EEffectActor;

	// Q
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEffectActor> DashClass1;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEffectActor> DashClass2;

	class AEffectActor* Dash1;
	class AEffectActor* Dash2;


	UFUNCTION(Server, Reliable)
	void Server_ChangeAttackState(EAttackState attackState);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_ChangeAttackState(EAttackState attackState);

private:
	bool bIsRun = false;

	// LMB
	int32 MaxCnt = 5;
	int32 ComboCnt = 0;
	float MontageEndTime = 0.6f;
	FTimerHandle ComboResetTimerHandle;
	float ComboResetDelay = 1.5f;

	// RMB
	bool bIsShield = false;

	// E
	bool bIsDashing = false;
	float DashSpeed = 1500.0f; // 돌진 속도
	float DashTimeElapsed = 0.0f;
	float DashTimer = 4.0f;
	FVector DashDirection;

	UFUNCTION()
	void OnDashCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void StopEDash();
	void ShieldBreak();

	// input
	void InputJump();
	void InputRun();
	void LMBAttack();
	void RMBAttack();
	void QAttack();
	void EAttack();
	
	// triggered
	void RMBTriggered();

	// completed
	void RMBCompleted();

	// spawnActor
	void LMBAttackActor();
	void EAttackActor();
};
