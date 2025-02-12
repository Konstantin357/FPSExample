// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ScoreManager.generated.h"

/**
 * 
 */
UCLASS()
class FPSEXAMPLE_API UScoreManager : public UObject
{
	GENERATED_BODY()
	
	FDateTime MatchStartTime = FDateTime();

	TMap<AFPSExampleCharacter*, int32> PickupScores;

public:
	void OnMatchStarted();

	void RegisterPickup(UPickupComponentHP* Pickup);
	void UnregisterPickup(UPickupComponentHP* Pickup);
	void RegisterCharacter(AFPSExampleCharacter* inCharacter);
	void UnregisterCharacter(AFPSExampleCharacter* inCharacter);

	FDateTime GetMatchStartTime();

private:
	UFUNCTION()
	void OnItemPickedUp(UPickupComponentHP* Pickup, AFPSExampleCharacter* PickupCharacter);
	UFUNCTION()
	void OnCharacterDied(AFPSExampleCharacter* Character);
};