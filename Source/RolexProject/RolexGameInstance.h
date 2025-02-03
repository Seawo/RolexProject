// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "BaseCharacter.h"
#include "RolexPlayerState.h"
#include "RolexGameInstance.generated.h"


DECLARE_DELEGATE_ThreeParams(FAddSession, int32, const FString&, const FString&);

UCLASS()
class ROLEXPROJECT_API URolexGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// called after GameInstance is initialized
	virtual void Init() override;
	
public:
	IOnlineSubsystem* OnlineSubsystem;
	
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	
	TSharedPtr<FOnlineSessionSearch> SessionSearched;

	FText RoomName;
	
	FDelegateHandle DestroySessionCompleteDelegateHandle;

public:
	void CreateSession(FName Name);
	void OnCreateSession(FName SessionName, bool bWasSuccessful);

	void FindSession();
	void OnFindSession(bool bWasSuccessful);

	void JoinOtherSession(int32 Index, FString Name);
	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void LeaveSession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FAddSession AddSession;

	UPROPERTY()
	TSubclassOf<ABaseCharacter> MainLevelPawn;

	// create session settings
public:
	int32 MaxPlayersNum;

	FString TravelLevel = FString("/Game/Rolex/PublicMap/PlayMap/PlayLevel?listen");;
	
public:
	// playerID - team
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, bool> PlayerTeam;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, TSubclassOf<ABaseCharacter>> PlayerHeroSelections;

};
