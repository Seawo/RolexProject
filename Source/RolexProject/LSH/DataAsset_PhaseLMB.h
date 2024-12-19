// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_PhaseLMB.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API UDataAsset_PhaseLMB : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDataAsset_PhaseLMB();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionName;						// 몽타주 섹션 이름
		
	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;							// 최대 콤보 카운트

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;								// 프레임 레이트

	UPROPERTY(EditAnywhere, Category = Name)
	TArray<float> EffectiveFrameCount;				// 입력 감지 프레임 배열
};
