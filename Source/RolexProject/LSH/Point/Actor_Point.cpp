// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_Point.h"


#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BaseCharacter.h"

// Sets default values
AActor_Point::AActor_Point()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetRelativeScale3D(FVector(20.0f, 20.0f, 4.0f));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -31.0f));
	MeshComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.01f));
}

// Called when the game starts or when spawned
void AActor_Point::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AActor_Point::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AActor_Point::OnOverlapEnd);
}

// Called every frame
void AActor_Point::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_Point::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AActor_Point::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);

	if (character and character->Data.Team == bIsATeam)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Point] OnOverlapEnd"));
	}
}

