// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExampleGameState.h"
#include "Net/UnrealNetwork.h"
#include "FPSExampleCharacter.h"
#include "FPSExampleSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "FPSExample.h"

static FString SaveGameName = FString("SaveName");
static int32 SaveGameSlotNumber = 0;
static TArray<FMatchResult> DummyMatchResults;

void AFPSExampleGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSExampleGameState, FPSMatchStarted);
	DOREPLIFETIME(AFPSExampleGameState, MatchTimeForUI);
	DOREPLIFETIME(AFPSExampleGameState, RegisteredCharacters);
}

void AFPSExampleGameState::BeginPlay()
{
	Super::BeginPlay();

	FString saveGameName = GetSaveGameName();

	if (UGameplayStatics::DoesSaveGameExist(saveGameName, SaveGameSlotNumber))
	{
		if (USaveGame* saveGame = UGameplayStatics::LoadGameFromSlot(saveGameName, SaveGameSlotNumber))
		{
			if (UFPSExampleSaveGame* saveGameCast = Cast<UFPSExampleSaveGame>(saveGame))
			{
				SaveGame = saveGameCast;
			}
		}
	}
	else
	{
		if (USaveGame* saveGame = UGameplayStatics::CreateSaveGameObject(UFPSExampleSaveGame::StaticClass()))
		{
			if (UFPSExampleSaveGame* saveGameCast = Cast<UFPSExampleSaveGame>(saveGame))
			{
				SaveGame = saveGameCast;
			}
		}
	}

	if (SaveGame == nullptr)
	{
		UE_LOG(LogFPSExample, Error, TEXT("AFPSExampleGameState::BeginPlay: SaveGame initialization error."));
	}
}

void AFPSExampleGameState::SetMatchStarted(bool inMatchStarted)
{
	FPSMatchStarted = inMatchStarted;
}

bool AFPSExampleGameState::IsFPSMatchStarted()
{
	return FPSMatchStarted;
}

void AFPSExampleGameState::SetMatchTimeForUI(int32 inTime)
{
	MatchTimeForUI = inTime;
}

void AFPSExampleGameState::RegisterCharacter(AFPSExampleCharacter* inCharacter)
{
	RegisteredCharacters.AddUnique(inCharacter);
}

const TArray<AFPSExampleCharacter*>& AFPSExampleGameState::GetRegisteredCharacters()
{
	return RegisteredCharacters;
}

AFPSExampleCharacter* AFPSExampleGameState::GetLocalCharacter()
{
	for (AFPSExampleCharacter* character : RegisteredCharacters)
	{
		if (character->IsValidLowLevel())
		{
			if (AController* controller = character->GetController())
			{
				if (controller->IsLocalPlayerController())
				{
					return character;
				}
			}
		}
	}
	return nullptr;
}

#if WITH_EDITOR
int32 AFPSExampleGameState::GetWorldIndexForInEditorSave()
{
	if (const UWorld* myWorld = GetWorld())
	{
		const TIndirectArray<FWorldContext>& worldList = GEngine->GetWorldContexts();
		for (const FWorldContext& Context : worldList)
		{
			if (const UWorld* world = Context.World())
			{
				if (myWorld == world)
				{
					return Context.PIEInstance;
				}
			}
		}
	}
	return INDEX_NONE;
}
#endif // WITH_EDITOR
FString AFPSExampleGameState::GetSaveGameName()
{
#if WITH_EDITOR
	FString saveGameName = SaveGameName + FString::FromInt(GetWorldIndexForInEditorSave());
	return saveGameName;
#else
	return SaveGameName;
#endif // WITH_EDITOR
}


void AFPSExampleGameState::ProcessNewMatchresult(const FMatchResult& inMatchResult)
{
	CurrentMatchResult = inMatchResult;

	FString saveGameName = GetSaveGameName();

	if (SaveGame->IsValidLowLevel())
	{
		SaveGame->AddNewMatchResult(inMatchResult);
		UGameplayStatics::SaveGameToSlot(SaveGame, saveGameName, SaveGameSlotNumber);
		MatchResultsReady.Broadcast(SaveGame->GetSavedMatchResults(), CurrentMatchResult);
	}
	else
	{
		UE_LOG(LogFPSExample, Error, TEXT("AFPSExampleGameState::ProcessNewMatchresult: SaveGame object invalid."));
	}
}

void AFPSExampleGameState::SetLocalCharacterPickupScore(int32 inScore)
{
	LocalCharacterPickupScore = inScore;
}

const TArray<FMatchResult>& AFPSExampleGameState::MatchResultHistory()
{
	if (SaveGame->IsValidLowLevel())
	{
		return SaveGame->GetSavedMatchResults();
	}
	else
	{
		UE_LOG(LogFPSExample, Error, TEXT("AFPSExampleGameState::MatchResultHistory: SaveGame object invalid."));
	}
	return DummyMatchResults;
}