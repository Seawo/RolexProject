// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectActor.h"
#include "Components/SphereComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEffectActor::AEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌 컴포넌트 생성
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = CollisionComp;
	CollisionComp->InitSphereRadius(100.0f); // 범위 조정
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComp"));
	NiagaraComp->SetupAttachment(RootComponent);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);

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

// 충돌 났을때
void AEffectActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		// 효과 추가: 나이아가라 변경, 파티클 추가 등
		UE_LOG(LogTemp, Log, TEXT("Effect collided with: %s"), *OtherActor->GetName());

		if (NiagaraCollusionEffect)
		{

		}
		else if (ParticleCollusionEffect)
		{

		}

	}
}

