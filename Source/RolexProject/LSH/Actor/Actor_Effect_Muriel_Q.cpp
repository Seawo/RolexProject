// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Muriel_Q.h"

#include "BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "RolexPlayerState.h"


AActor_Effect_Muriel_Q::AActor_Effect_Muriel_Q()
{
	PrimaryActorTick.bCanEverTick = true;

	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);

}

void AActor_Effect_Muriel_Q::BeginPlay()
{
	Super::BeginPlay();

	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &AActor_Effect_Muriel_Q::OnOverlapBegin);
}

void AActor_Effect_Muriel_Q::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActor_Effect_Muriel_Q::UpdateLocation(float DeltaTime)
{
	
}

void AActor_Effect_Muriel_Q::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (OtherActor == GetOwner()) return;

	AActor_Effect* effect = Cast<AActor_Effect>(OtherActor);
	if (effect)
	{
		return;
	}


	ABaseCharacter* character = Cast<ABaseCharacter>(OtherActor);
	ABaseCharacter* owner = Cast<ABaseCharacter>(GetOwner());
	if (character)
	{
		// 적팀인 경우
		if (character->Data.Team != owner->Data.Team)
		{
			if (character->Data.Hp <= 0)
			{
				owner->RolexPS->PlayerData.KillCount++;
			}
			else if (character->Data.Hp < 60)
			{
				owner->RolexPS->PlayerData.Damage += character->Data.Hp;
				character->ModifyHP(-character->Data.Hp);
			}
			else
			{
				character->ModifyHP(-60);
				owner->RolexPS->PlayerData.Damage += 60;
			}
		}
		// 아군인 경우
		else if (character->Data.Team == owner->Data.Team)
		{
			// 실드 추가
			character->ModifyShield(1);
		}
	}

	SetLifeSpan(0.5f);
}
