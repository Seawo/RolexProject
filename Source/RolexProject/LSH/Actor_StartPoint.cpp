// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor_StartPoint.h"

#include "Components/BoxComponent.h"

// Sets default values
AActor_StartPoint::AActor_StartPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetRelativeScale3D(FVector(20.0f, 20.0f, 4.0f));
}

// Called when the game starts or when spawned
void AActor_StartPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_StartPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AActor_StartPoint::SpawnPoint()
{
	float random = FMath::RandRange(-300 , 300);

	FVector SpawnLocation = GetActorLocation() + FVector(random, random, 0);

	return SpawnLocation;
}

