// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Engine/Engine.h"

URolexGameInstance::URolexGameInstance()
{
	OnlineSubsystem = IOnlineSubsystem::Get();
	
	SessionInterface = OnlineSubsystem->GetSessionInterface();
	
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URolexGameInstance::OnCreateSession);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URolexGameInstance::OnFindSession);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &URolexGameInstance::OnJoinSession);
}

void URolexGameInstance::CreateSession()
{
	if (OnlineSubsystem)
	{
		if (SessionInterface.IsValid())
		{
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = false;
			SessionSettings.bUsesPresence = true;
			SessionSettings.NumPublicConnections = 5;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bAllowJoinInProgress = true;
			SessionSettings.bAllowJoinViaPresence = false;
			
			SessionInterface->CreateSession(0, FName("SessinonName"), SessionSettings);
		}
	}
}

void URolexGameInstance::OnCreateSession(FName SessionName, bool bWasSuccessful)
{
}

void URolexGameInstance::FindSession()
{
	if (OnlineSubsystem)
	{
		if (SessionInterface.IsValid())
		{
			TArray<FOnlineSessionSearchResult> SessionSearchResults;
			
			FOnlineSessionSearch SearchSettings;
			SearchSettings.MaxSearchResults = 5;
			SearchSettings.QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		}
	}
}

void URolexGameInstance::OnFindSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		
	}
}

void URolexGameInstance::JoinSession(int32 SessionIndex)
{
	if (SessionInterface)
	{
		TArray<FOnlineSessionSearchResult> SessionSearchResults;
		SessionInterface->JoinSession(0, FName("SessonName"), SessionSearchResults[SessionIndex]);
	}
}

void URolexGameInstance::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

