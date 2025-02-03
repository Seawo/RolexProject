// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCamera.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ACharacterCamera::ACharacterCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	RootComponent = SkeletalMeshComponent;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(SkeletalMeshComponent);
	
	CaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraComponent"));
	CaptureComponent->SetupAttachment(SpringArm);
	
}

// Called when the game starts or when spawned
void ACharacterCamera::BeginPlay()
{
	Super::BeginPlay();

	// render only the actor components, not the background
	CaptureComponent->ShowOnlyActorComponents(this, true);
}

// Called every frame
void ACharacterCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

