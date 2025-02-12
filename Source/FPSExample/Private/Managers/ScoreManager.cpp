// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreManager.h"
#include "PickupComponentHP.h"
#include "FPSExampleCharacter.h"
#include "FPSExamplePlayerController.h"
#include "FPSExampleGameState.h"

void UScoreManager::OnMatchStarted()
{
	MatchStartTime = FDateTime::UtcNow();
}

void UScoreManager::RegisterPickup(UPickupComponentHP* Pickup)
{
	Pickup->OnPickup.AddDynamic(this, &UScoreManager::OnItemPickedUp);
}

void UScoreManager::UnregisterPickup(UPickupComponentHP* Pickup)
{
	Pickup->OnPickup.RemoveDynamic(this, &UScoreManager::OnItemPickedUp);
}

void UScoreManager::RegisterCharacter(AFPSExampleCharacter* inCharacter)
{
	inCharacter->OnCharacterDied.AddDynamic(this, &UScoreManager::OnCharacterDied);
}

void UScoreManager::UnregisterCharacter(AFPSExampleCharacter* inCharacter)
{
	inCharacter->OnCharacterDied.RemoveDynamic(this, &UScoreManager::OnCharacterDied);
}

FDateTime UScoreManager::GetMatchStartTime()
{
	return MatchStartTime;
}

void UScoreManager::OnItemPickedUp(UPickupComponentHP* Pickup, AFPSExampleCharacter* PickupCharacter)
{

	if (PickupCharacter->IsValidLowLevel())
	{
		int32& characterScore = PickupScores.FindOrAdd(PickupCharacter);
		++characterScore;

		if (AController* controller = PickupCharacter->GetController())
		{
			if (AFPSExamplePlayerController* playerController = Cast<AFPSExamplePlayerController>(controller))
			{
				playerController->UpdateLocalCharacterPickupScore(characterScore);
			}
		}
	}
	Pickup->OnPickup.RemoveDynamic(this, &UScoreManager::OnItemPickedUp);
}

void UScoreManager::OnCharacterDied(AFPSExampleCharacter* Character)
{
	Character->OnCharacterDied.RemoveDynamic(this, &UScoreManager::OnCharacterDied);

	FDateTime characterMatchEndTime = FDateTime::UtcNow();
	FTimespan characterMatchPlayTime = characterMatchEndTime - MatchStartTime;
	int32 countHPPickups = PickupScores.FindOrAdd(Character);

	FMatchResult matchResult = FMatchResult(true, characterMatchPlayTime, countHPPickups);
	if (AController* controller = Character->GetController())
	{
		if (AFPSExamplePlayerController* playerController = Cast<AFPSExamplePlayerController>(controller))
		{
			playerController->UpdateMatchResults(matchResult);
		}
	}
}