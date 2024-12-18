// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Character_Phase.h"

ACharacter_Phase::ACharacter_Phase()
{
	PrimaryActorTick.bCanEverTick = true;

	Data.RoleType = ERoleType::Dealer;
	Data.Name = "Phase";
	Data.Team = true;
	Data.MaxHp = 250.0f;
	Data.Hp = 250.0f;
	Data.Shield = 0.0f;
	Data.Speed = 400.0f;
	Data.Power = 20.0f;
}

void ACharacter_Phase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacter_Phase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_Phase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void ACharacter_Phase::QSkill()
{
}

void ACharacter_Phase::ESkill()
{
}

void ACharacter_Phase::LMB()
{
}

void ACharacter_Phase::RMB()
{
}
