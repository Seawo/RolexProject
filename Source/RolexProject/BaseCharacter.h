// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


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



	void InputMove(const struct FInputActionValue& inputValue);
	void InputRotation(const struct FInputActionValue& inputValue);
};
