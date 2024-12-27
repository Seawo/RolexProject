// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AEffectActor::AEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 컴포넌트 생성
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(CollisionRadius); // 범위 조정
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (bIsDamage)
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnOverlapBegin);
	}
	

	SetLifeSpan(DestroyTime);
}

// Called every frame
void AEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MoveDir.IsNearlyZero())
	{
		
		FVector newVector = GetActorLocation() + (MoveDir * ThrowSpeed * DeltaTime);
		SetActorLocation(newVector);

	}
}

// 생성될때,
void AEffectActor::InitializeEffect(UNiagaraSystem* niagaraSystem, FVector scale)
{
	if (niagaraSystem)
	{
		// niagara system setting
		NiagaraComp->SetAsset(niagaraSystem);
		NiagaraComp->SetWorldScale3D(scale);
		NiagaraComp->Activate(true);
	}
}

void AEffectActor::InitializeEffect(UParticleSystem* particleEffect, FVector effectScale)
{
	if (particleEffect)
	{
		ParticleComp->SetTemplate(particleEffect);
		ParticleComp->SetWorldScale3D(effectScale);
		ParticleComp->Activate(true);
	}
}

void AEffectActor::InititalizeThrowStone(const FVector& dir, float speed)
{
	// 던질때 크기 조절??
	//CollisionComp->InitSphereRadius(500.0f);
	//StaticMeshComp->SetWorldScale3D(FVector(30.0f));

	MoveDir = dir.GetSafeNormal();
	ThrowSpeed = speed;
}


// 충돌 났을때
void AEffectActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()|| OtherActor->IsA(AEffectActor::StaticClass()))
	{
		return;
	}

	if (OtherActor && OtherActor != this)
	{
		// 효과 추가: 나이아가라 변경, 파티클 추가 등
		UE_LOG(LogTemp, Log, TEXT("Effect collided with: %s"), *OtherActor->GetName());

		if (NiagaraCollusionEffect)
		{

		}
		else if (ParticleCollusionEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ParticleCollusionEffect,
				OtherActor->GetActorLocation(),
				FRotator::ZeroRotator,
				true // 자동 크기 조정
			);
		}

		FTimerHandle handle;

		GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateLambda(
			[this]()
			{
				Destroy();
			}),
			0.2f, false);
	}
}

