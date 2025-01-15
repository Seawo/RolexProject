// Fill out your copyright notice in the Description page of Project Settings.


#include "UltimateBall.h"

#include "BaseCharacter.h"
#include "Fey.h"
#include "Chaos/Collision/ConvexContactPointUtilities.h"
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

	Type = FMath::RandBool();
	UMaterialInstanceDynamic* SelectedMaterial;

	if (Type)
		SelectedMaterial = HealBallMaterial;
	else
		SelectedMaterial = AttackBallMaterial;
	
	if (SelectedMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball Type: %s"), *SelectedMaterial->GetName());
		MeshComponent->SetMaterial(0, SelectedMaterial);
	}
	
	//InitialLifeSpan = 3.0f;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AUltimateBall::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3.0f);
	
	OwnCharacter = Cast<ABaseCharacter>(GetOwner());
	
	Sphere->OnComponentHit.AddDynamic(this, &AUltimateBall::OnHit);
}

// Called every frame
void AUltimateBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		Server_ActorPos(GetActorLocation());
	}
}

void AUltimateBall::Server_ActorPos_Implementation(FVector pos)
{
	SetActorLocation(pos);
	Multi_ActorPos(pos);
}

void AUltimateBall::Multi_ActorPos_Implementation(FVector pos)
{
	SetActorLocation(pos);
}

void AUltimateBall::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);
		
		// logic
		if (Character != nullptr)
		{
			if (Character == OwnCharacter) return;
			
			if (Character->Data.Team == OwnCharacter->Data.Team )
			{
				UE_LOG(LogTemp, Warning, TEXT("Heal Ball"));
				//Character->Data.Hp += 15.0f;
				Character->ModifyHP(15.0f);
			}
			if (Character->Data.Team != OwnCharacter->Data.Team)
			{
				UE_LOG(LogTemp, Warning, TEXT("Attack Ball"));
				//Character->Data.Hp -= 15.0f;
				Character->ModifyHP(-15.0f);
			}
		}
	}

	// bounce
	// FVector BounceDirection = NormalImpulse.GetSafeNormal();
	// ProjectileMovement->Velocity = BounceDirection * 1000.0f;
}


