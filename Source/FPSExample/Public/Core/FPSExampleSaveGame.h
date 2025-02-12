// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FPSETypes.h"
#include "FPSExampleSaveGame.generated.h"


struct FMatchResult;
/**
 * 
 */
UCLASS()
class FPSEXAMPLE_API UFPSExampleSaveGame : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TArray<FMatchResult> MatchResults;

public:
	void AddNewMatchResult(const FMatchResult& inMatchResult);
	void UpdateMatchResults(const TArray<FMatchResult>& inMatchResults);
	const TArray<FMatchResult>& GetSavedMatchResults();
};