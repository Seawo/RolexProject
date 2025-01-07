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

void URolexGameInstance::CreateSession(FName Name)
{
	if (OnlineSubsystem)
	{
		if (SessionInterface.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = false;
			SessionSettings->bUsesPresence = true;
			SessionSettings->NumPublicConnections = 5;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bAllowJoinViaPresence = false;

			// custom information
			//SessionSettings.Set(TEXT("RoomName"), Name.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			
			ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
			FUniqueNetIdRepl SteamNetId = *LocalPlayer->GetPreferredUniqueNetId();
			if (SteamNetId.IsValid())
				SessionInterface->CreateSession(*SteamNetId, Name, *SessionSettings);
			
		}
	}
}

void URolexGameInstance::OnCreateSession(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Session Success"));
		GetWorld()->ServerTravel(TEXT("Game/Rolex/Maps/Main?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Create Session Failed"));
	}
}

void URolexGameInstance::FindSession()
{
	if (OnlineSubsystem)
	{
		if (SessionInterface.IsValid())
		{			
			SearchSettings = MakeShared<FOnlineSessionSearch>();
			SearchSettings->bIsLanQuery = false;
			SearchSettings->MaxSearchResults = 100;
			SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			
			SessionInterface->FindSessions(0, SearchSettings.ToSharedRef());
		}
	}
}

void URolexGameInstance::OnFindSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		auto results = SearchSettings->SearchResults;

		for (int32 i = 0; i < results.Num(); i++)
		{
			FOnlineSessionSearchResult SearchResult = results[i];
			FString OwnerName = SearchResult.Session.OwningUserName;
			//FString SessionName = SearchResult.Session.
				
		}
		
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

