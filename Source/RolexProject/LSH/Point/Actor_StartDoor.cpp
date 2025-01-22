// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Point/Actor_StartDoor.h"

// Sets default values
AActor_StartDoor::AActor_StartDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActor_StartDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_StartDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

