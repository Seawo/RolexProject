// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Point/Actor_FallingPoint.h"

#include "Components/BoxComponent.h"
#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AActor_FallingPoint::AActor_FallingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	SetRootComponent(Collision);
}

// Called when the game starts or when spawned
void AActor_FallingPoint::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AActor_FallingPoint::OnOverlapBegin);
}

// Called every frame
void AActor_FallingPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_FallingPoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	if (character)
	{
		character->ChangeState(EMoveState::Die, character->stateMontages["Die"]);
		if (Particle)
		{
			
			FVector location = character->GetActorLocation() + Location;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, location);
		}
	}
}

