// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "FPSETypes.h"
#include "FPSExampleGameState.generated.h"

class AFPSExampleCharacter;
class UFPSExampleSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMatchResultsReady, const TArray<FMatchResult>&, MatchResultHistory, const FMatchResult&, CurrentMatchResult);
/**
 * 
 */
UCLASS()
class FPSEXAMPLE_API AFPSExampleGameState : public AGameState
{
	GENERATED_BODY()

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;
// Replicated part
private:
	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool FPSMatchStarted = false;
	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MatchTimeForUI = 0;

	UPROPERTY(Replicated)
	TArray<AFPSExampleCharacter*> RegisteredCharacters;

	UPROPERTY()
	UFPSExampleSaveGame* SaveGame;

public:
	void SetMatchStarted(bool inMatchStarted);
	bool IsFPSMatchStarted();

	void SetMatchTimeForUI(int32 inTime);

	void RegisterCharacter(AFPSExampleCharacter* inCharacter);
	UFUNCTION(BlueprintPure)
	const TArray<AFPSExampleCharacter*>& GetRegisteredCharacters();
	UFUNCTION(BlueprintPure)
	AFPSExampleCharacter* GetLocalCharacter();

// Client part
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FMatchResult CurrentMatchResult;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 LocalCharacterPickupScore = 0;

// Fix for testing in editor, helps create individual save files for each PIE window
#if WITH_EDITOR
	int32 GetWorldIndexForInEditorSave();
#endif // WITH_EDITOR
	FString GetSaveGameName();

public:
	void ProcessNewMatchresult(const FMatchResult& inMatchResult);
	void SetLocalCharacterPickupScore(int32 inScore);

	UFUNCTION(BlueprintPure)
	const TArray<FMatchResult>& MatchResultHistory();

	UPROPERTY(BlueprintAssignable)
	FMatchResultsReady MatchResultsReady;
};