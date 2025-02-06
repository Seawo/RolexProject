// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioManager.h"

#include "Components/AudioComponent.h"

// Sets default values
AAudioManager::AAudioManager()
{
 	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();
	
}

