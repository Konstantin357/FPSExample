// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSEUtils.h"
#include "Kismet/GameplayStatics.h"
#include "FPSExampleGameInstance.h"
#include "FPSExampleGameMode.h"
#include "FPSExampleGameState.h"

UFPSExampleGameInstance* UFPSEUtils::GetFPSExampleGameInstance()
{
	UFPSExampleGameInstance* returnGameInstance = nullptr;

	if (nullptr != GEngine)
	{
		const TIndirectArray<FWorldContext>& worldList = GEngine->GetWorldContexts();
		for (const FWorldContext& Context : worldList)
		{
			if (const UWorld* world = Context.World())
			{
				if (UGameInstance* gameInstance = world->GetGameInstance())
				{
					returnGameInstance = Cast<UFPSExampleGameInstance>(gameInstance);

					if (nullptr != returnGameInstance)
						break;
				}
			}
		}

		if (nullptr == returnGameInstance)
		{
			if (nullptr != GEngine->GameViewport)
			{
				if (UGameInstance* gameInstance = GEngine->GameViewport->GetGameInstance())
				{
					returnGameInstance = Cast<UFPSExampleGameInstance>(gameInstance);
				}
			}
		}
	}

	if (nullptr == returnGameInstance && nullptr != GWorld)
	{
		if (UGameInstance* gameInstance = GWorld->GetGameInstance())
		{
			returnGameInstance = returnGameInstance = Cast<UFPSExampleGameInstance>(gameInstance);
		}
	}

	return returnGameInstance;
}

AFPSExampleGameMode* UFPSEUtils::GetFPSExampleGameMode()
{
	if (UFPSExampleGameInstance* gameInstance = UFPSEUtils::GetFPSExampleGameInstance())
	{
		if (UWorld* world = gameInstance->GetWorld())
		{
			if (AGameModeBase* gameModeBase = UGameplayStatics::GetGameMode(world))
			{
				if (AFPSExampleGameMode* gameMode = Cast<AFPSExampleGameMode>(gameModeBase))
				{
					return gameMode;
				}
			}
		}
	}
	return nullptr;
}

AFPSExampleGameState* UFPSEUtils::GetFPSExampleGameState()
{
	if (UFPSExampleGameInstance* gameInstance = UFPSEUtils::GetFPSExampleGameInstance())
	{
		if (UWorld* world = gameInstance->GetWorld())
		{
			if (AGameStateBase* gameStateBase = UGameplayStatics::GetGameState(world))
			{
				if (AFPSExampleGameState* gameState = Cast<AFPSExampleGameState>(gameStateBase))
				{
					return gameState;
				}
			}
		}
	}
	return nullptr;
}

bool UFPSEUtils::IsServerOrStandalone()
{
	if (UFPSExampleGameInstance* gameInstance = UFPSEUtils::GetFPSExampleGameInstance())
	{
		if (UWorld* world = gameInstance->GetWorld())
		{
			ENetMode netMode = world->GetNetMode();
			return netMode < ENetMode::NM_Client;
		}
	}
	return false;
}

UFPSExampleGameInstance* UFPSEUtils::GetFPSExampleGameInstanceWithContext(UObject* contextObject)
{
	if (contextObject->IsValidLowLevel())
	{
		if (const UWorld* world = contextObject->GetWorld())
		{
			if (UGameInstance* gameInstance = world->GetGameInstance())
			{
				if (UFPSExampleGameInstance* gameInstanceCast = Cast<UFPSExampleGameInstance>(gameInstance))
				{
					return gameInstanceCast;
				}
			}
		}
	}
	return nullptr;
}

AFPSExampleGameMode* UFPSEUtils::GetFPSExampleGameModeWithContext(UObject* contextObject)
{
	if (contextObject->IsValidLowLevel())
	{
		if (const UWorld* world = contextObject->GetWorld())
		{
			if (AGameModeBase* gameModeBase = UGameplayStatics::GetGameMode(world))
			{
				if (AFPSExampleGameMode* gameMode = Cast<AFPSExampleGameMode>(gameModeBase))
				{
					return gameMode;
				}
			}
		}
	}
	return nullptr;
}

AFPSExampleGameState* UFPSEUtils::GetFPSExampleGameStateWithContext(UObject* contextObject)
{
	if (contextObject->IsValidLowLevel())
	{
		if (const UWorld* world = contextObject->GetWorld())
		{
			if (AGameStateBase* gameStateBase = UGameplayStatics::GetGameState(world))
			{
				if (AFPSExampleGameState* gameState = Cast<AFPSExampleGameState>(gameStateBase))
				{
					return gameState;
				}
			}
		}
	}
	return nullptr;
}

AFPSExampleCharacter* UFPSEUtils::GetLocalFPSExampleCharacterWithContext(UObject* contextObject)
{
	if (contextObject->IsValidLowLevel())
	{
		if (AFPSExampleGameState* gameState = GetFPSExampleGameStateWithContext(contextObject))
		{
			if (AFPSExampleCharacter* character = gameState->GetLocalCharacter())
			{
				return character;
			}
		}
	}
	return nullptr;
}