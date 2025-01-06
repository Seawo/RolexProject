// Fill out your copyright notice in the Description page of Project Settings.


#include "LSH/Actor/Actor_Effect_Muriel_Q.h"

#include "BaseCharacter.h"

AActor_Effect_Muriel_Q::AActor_Effect_Muriel_Q()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AActor_Effect_Muriel_Q::BeginPlay()
{
	Super::BeginPlay();
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
			character->ModifyHP(-1);
		}
		// 아군인 경우
		else if (character->Data.Team == owner->Data.Team)
		{
			// 실드 추가
			character->ModifyShield(1);
		}
	}
}
