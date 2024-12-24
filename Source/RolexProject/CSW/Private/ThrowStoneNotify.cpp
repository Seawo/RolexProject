// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowStoneNotify.h"
#include "Character_Rampage.h"

void UThrowStoneNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp)
	{
		ACharacter_Rampage* rampage = Cast<ACharacter_Rampage>(MeshComp->GetOwner());
		if (rampage)
		{
			rampage->ThrowStone();
		}
	}

}
