// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor_StartPoint.h"

#include "Components/BoxComponent.h"
#include "BaseCharacter.h"

// Sets default values
AActor_StartPoint::AActor_StartPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

void AActor_StartPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("[StartPoint] OnOverlapBegin"));
}

void AActor_StartPoint::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("[StartPoint] OnOverlapEnd"));
}

