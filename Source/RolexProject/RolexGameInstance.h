// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "RolexGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROLEXPROJECT_API URolexGameInstance : public UGameInstance
{
	GENERATED_BODY()

	URolexGameInstance();
	
public:
	IOnlineSubsystem* OnlineSubsystem;
	
	IOnlineSessionPtr SessionInterface;

	
public:
	void CreateSession();
	void OnCreateSession(FName SessionName, bool bWasSuccessful);

	void FindSession();
	void OnFindSession(bool bWasSuccessful);

	void JoinSession(int32 SessionIndex);
	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
