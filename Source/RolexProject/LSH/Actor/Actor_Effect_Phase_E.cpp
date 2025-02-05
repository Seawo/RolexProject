// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Phase_E.h"
#include "Components/SphereComponent.h"

#include "Character_Phase.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"

AActor_Effect_Phase_E::AActor_Effect_Phase_E()
{
	PrimaryActorTick.bCanEverTick = true;

	ShieldCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCollision"));
	ShieldCollision->SetupAttachment(RootComponent);

	ShieldCollision->SetRelativeScale3D(FVector(4.8f));
}

void AActor_Effect_Phase_E::BeginPlay()
{
	Super::BeginPlay();

	ShieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_Phase_E::OnOverlapBegin);
	Onwer = Cast<ABaseCharacter>(GetOwner());

	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer,
			FTimerDelegate::CreateLambda([this]() 
			{
				if (Onwer)
				{
					Onwer->Data.Shield = 0;
				}
				Destroy(); 
				NiagaraComponent->Deactivate();
			}), Phase->ESkillDuration, false);
}

void AActor_Effect_Phase_E::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Onwer and Onwer->Data.Shield <= 0)
	{
		Destroy();
		NiagaraComponent->Deactivate();
	}
}

void AActor_Effect_Phase_E::UpdateLocation(float DeltaTime)
{
	SetActorLocation(Phase->GetActorLocation());
	SetActorRotation(Phase->TpsCamComp->GetForwardVector().Rotation());
}

void AActor_Effect_Phase_E::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() or OtherActor == this)
	{
		return;
	}
	// LMB RMB가 E스킬에 막히는거 방치용
	AActor_Effect* effect = Cast<AActor_Effect>(OtherActor);
	if (effect)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[E] Other : %s, Owner : %s"),
		*OtherActor->GetName(), *GetOwner()->GetName());
	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	ABaseCharacter* onwer = Cast<ABaseCharacter>(GetOwner());

	if (character && character->Data.Team != onwer->Data.Team)
	{
		if (bIsOverlap) return;	// 중복 방지
		bIsOverlap = true;

		if (character and OtherActor != this)
		{
			UE_LOG(LogTemp, Log, TEXT("Shield Overlap Begin"));


			FVector myLoc = GetActorLocation();
			FVector targetLoc = character->GetActorLocation();
			FVector dir = (targetLoc - myLoc).GetSafeNormal();

			// 힘적용
			character->LaunchCharacter(dir * 2000, true, true);
		}
	}
}
