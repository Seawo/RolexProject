// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Engine/Engine.h"


void URolexGameInstance::Init()
{
	Super::Init();

	OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem Exists"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem does not Exists"));
	}
	
	SessionInterface = OnlineSubsystem->GetSessionInterface();
	
	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &URolexGameInstance::OnCreateSession);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &URolexGameInstance::OnFindSession);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &URolexGameInstance::OnJoinSession);
}

void URolexGameInstance::CreateSession(FName Name)
{
	if (OnlineSubsystem)
	{
		FName SubsystemName = OnlineSubsystem->GetSubsystemName();
		UE_LOG(LogTemp, Warning, TEXT("Subsystem Name: %s"), *SubsystemName.ToString());
		
		if (SessionInterface.IsValid())
		{
			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = SubsystemName.IsEqual(FName(TEXT("NULL")));
			SessionSettings->bUseLobbiesIfAvailable = true;
			//SessionSettings->bUsesPresence = true; 
			SessionSettings->NumPublicConnections = 4;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinInProgress = true;
			//SessionSettings->bAllowJoinViaPresence = false;

			// custom information
			SessionSettings->Set(TEXT("RoomName"), Name.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			
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
		GetWorld()->ServerTravel(TEXT("/Game/Rolex/Map/WaitingRoom?listen"));
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
			SessionSearched = MakeShared<FOnlineSessionSearch>();
			FName SubsystemName = OnlineSubsystem->GetSubsystemName();
			UE_LOG(LogTemp, Warning, TEXT("String Subsystem Name: %s"), *SubsystemName.ToString());
			SessionSearched->bIsLanQuery = SubsystemName.IsEqual(FName(TEXT("NULL")));
			SessionSearched->MaxSearchResults = 100;
			SessionSearched->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
			
			SessionInterface->FindSessions(0, SessionSearched.ToSharedRef());
		}
	}
}

void URolexGameInstance::OnFindSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSession Succeed"));
		UE_LOG(LogTemp, Warning, TEXT("Number of search results: %d"), SessionSearched->SearchResults.Num());
		auto results = SessionSearched->SearchResults;

		for (int32 i = 0; i < results.Num(); i++)
		{
			FOnlineSessionSearchResult SearchResult = results[i];
			FString SessionId = SearchResult.GetSessionIdStr();
			FString OwnerName = SearchResult.Session.OwningUserName;
			FString SessionName;
			SearchResult.Session.SessionSettings.Get(TEXT("RoomName"), SessionName);

			UE_LOG(LogTemp, Warning, TEXT("Session Found: %s"), *SessionName);
			
			AddSession.ExecuteIfBound(i, OwnerName, SessionName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSession Failed"));
	}
}

void URolexGameInstance::JoinOtherSession(int32 Index, FString Name)
{
	if (SessionInterface)
	{
		FOnlineSessionSearchResult SearchResult = SessionSearched->SearchResults[Index];
		SessionInterface->JoinSession(0, FName(Name), SearchResult);
	}
}

void URolexGameInstance::OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Join success"));

		FString ConnectInfo;
		if (SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
		{
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			PlayerController->ClientTravel(ConnectInfo, TRAVEL_Absolute);
		}
	}
}






