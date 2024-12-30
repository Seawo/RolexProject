// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateBall.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AUltimateBall::AUltimateBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Ball = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	RootComponent = Ball;
}

// Called when the game starts or when spawned
void AUltimateBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUltimateBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

