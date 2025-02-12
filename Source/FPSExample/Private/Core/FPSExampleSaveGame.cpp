// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExampleSaveGame.h"

#define MAXIMUM_NUMBER_OF_SAVED_MATCH_RESULTS 10

void UFPSExampleSaveGame::AddNewMatchResult(const FMatchResult& inMatchResult)
{
	MatchResults.Add(inMatchResult);
	MatchResults.Sort([](const FMatchResult& A, const FMatchResult& B)
	{
		return (A.TimeSurvived != B.TimeSurvived) ? (A.TimeSurvived > B.TimeSurvived) : (A.HPPickupNumber > B.HPPickupNumber);
	});
	while (MatchResults.Num() > MAXIMUM_NUMBER_OF_SAVED_MATCH_RESULTS)
	{
		MatchResults.RemoveAt(MAXIMUM_NUMBER_OF_SAVED_MATCH_RESULTS);
	}
}

void UFPSExampleSaveGame::UpdateMatchResults(const TArray<FMatchResult>& inMatchResults)
{
	MatchResults = inMatchResults;
}

const TArray<FMatchResult>& UFPSExampleSaveGame::GetSavedMatchResults()
{
	return MatchResults;
}