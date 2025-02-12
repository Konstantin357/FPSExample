// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPSETypes.generated.h"

USTRUCT(BlueprintType)
struct FMatchResult
{
	GENERATED_BODY()

	FMatchResult()
	{
		IsValid = false;
		TimeSurvived = FTimespan();
		HPPickupNumber = 0;
	}

	FMatchResult(bool inIsValid, FTimespan inTimeSurvived, int32 inHPPickupNumber)
	{
		IsValid = inIsValid;
		TimeSurvived = inTimeSurvived;
		HPPickupNumber = inHPPickupNumber;
	}

	UPROPERTY(SaveGame, BlueprintReadOnly)
	bool IsValid = false;
	UPROPERTY(SaveGame, BlueprintReadOnly)
	FTimespan TimeSurvived = FTimespan();
	UPROPERTY(SaveGame, BlueprintReadOnly)
	int32 HPPickupNumber = 0;
};
/**
 * 
 */
UCLASS()
class FPSEXAMPLE_API UFPSETypes : public UObject
{
	GENERATED_BODY()
	
};
