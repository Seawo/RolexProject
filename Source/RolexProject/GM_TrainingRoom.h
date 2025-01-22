// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_TrainingRoom.generated.h"

//// 거점 점령 상태를 알려주는 Enum값
//UENUM(BlueprintType)
//enum class EOccupation : uint8
//{
//	None,				// 점령 안된 상태
//	TeamA,				// A팀점령
//	TeamB,				// B팀점령
//};
//
//
//// 거점 결과를 알려주는 Enum값
//UENUM(BlueprintType)
//enum class EResult : uint8
//{
//	None,
//	AWin,
//	BWin,
//	draw,
//};
//
//UENUM(BlueprintType)
//enum class EClashing : uint8
//{
//	None,				// 충돌 없는 상태	
//	Clash,				// A팀과 B팀이 충돌하는 상태
//};

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API AGM_TrainingRoom : public AGameModeBase
{
	GENERATED_BODY()

};
