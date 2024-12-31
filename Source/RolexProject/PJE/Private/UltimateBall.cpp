// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateBall.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AUltimateBall::AUltimateBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	MeshComponent->SetupAttachment(Sphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 1.f;
	ProjectileMovement->Bounciness = 1.f;
}

// Called when the game starts or when spawned
void AUltimateBall::BeginPlay()
{
	Super::BeginPlay();

	//Ball->OnComponentHit.AddDynamic(this, &AUltimateBall::OnHit);

	GetWorld()->GetTimerManager().SetTimer(
		DestroyTimerHandle, this, &AUltimateBall::DestroyBall, 3.0f, false);
}

// Called every frame
void AUltimateBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUltimateBall::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
}

void AUltimateBall::DestroyBall()
{
	Destroy();
}

