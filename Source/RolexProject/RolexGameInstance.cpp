// Fill out your copyright notice in the Description page of Project Settings.


#include "RolexGameInstance.h"

#include "LobbyUI.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Engine/Engine.h"
#include "OnlineSubsystem.h"

#include "RolexPlayerController.h"

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
		GetWorld()->ServerTravel(TEXT("/Game/Rolex/Map/WaitingRoom_MedivalAsia?listen"));
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
			// use MakeShared when asynchronous operations are involved: finding sessions is asynchronous operation
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
	if (!IsValid(this))
		return;
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSession Succeed"));
		UE_LOG(LogTemp, Warning, TEXT("Number of search results: %d"), SessionSearched->SearchResults.Num());

		// get the created sessions and add to the SessionScrollBox
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

void URolexGameInstance::LeaveSession()
{
	FName name = FName(RoomName.ToString());
	if (SessionInterface and SessionInterface->GetNamedSession(name))
	{ // NAME_GameSession
		ARolexPlayerController* pc = Cast<ARolexPlayerController>(GetWorld()->GetFirstPlayerController());

		if (pc and pc->HasAuthority())
		{
			// 서버일 경우
			SessionInterface->DestroySession(name, FOnDestroySessionCompleteDelegate::CreateLambda(
				[this](FName SessionName, bool bWasSuccessful)
				{
					UE_LOG(LogTemp, Log, TEXT("Listen Server: Session %s destroyed: %s"), *SessionName.ToString(), bWasSuccessful ? TEXT("Success") : TEXT("Failure"));
					GetWorld()->ServerTravel("/Game/Rolex/Map/Lobby_MedivalAsia?listen");
				}));
		}
		else
		{
			// 클라인 경우
			pc->ClientTravel("/Game/Rolex/Map/Lobby_MedivalAsia", ETravelType::TRAVEL_Absolute);
		}

		// 모든 클라이언트 내보내기
		//for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		//{
		//	APlayerController* PC = Iterator->Get();
		//	if (PC && !PC->IsLocalController()) // 로컬 플레이어(서버) 제외
		//	{
		//		PC->ClientTravel("/Game/Rolex/Map/Lobby_MedivalAsia", ETravelType::TRAVEL_Absolute);
		//	}
		//}

		// 세션 삭제 후 서버 이동
		/*SessionInterface->DestroySession(name, FOnDestroySessionCompleteDelegate::CreateLambda(
			[this](FName SessionName, bool bWasSuccessful)
			{
				UE_LOG(LogTemp, Log, TEXT("Listen Server: Session %s destroyed: %s"), *SessionName.ToString(), bWasSuccessful ? TEXT("Success") : TEXT("Failure"));
				GetWorld()->ServerTravel("/Game/Rolex/Map/Lobby_MedivalAsia?listen");
			}));*/
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SessionInterface is not valid"));
	}
}

void URolexGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	// 세션 파괴 성공시 메인 메뉴로 이동
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy Session Success"));
		UGameplayStatics::OpenLevel(this, FName("Lobby"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Destroy Session Failed"));
	}
}