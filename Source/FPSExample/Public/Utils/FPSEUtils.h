// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPSEUtils.generated.h"

class UFPSExampleGameInstance;
class AFPSExampleGameMode;
class AFPSExampleGameState;

/**
 * 
 */
UCLASS()
class FPSEXAMPLE_API UFPSEUtils : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static UFPSExampleGameInstance* GetFPSExampleGameInstance();

	UFUNCTION(BlueprintPure)
	static AFPSExampleGameMode* GetFPSExampleGameMode();

	UFUNCTION(BlueprintPure)
	static AFPSExampleGameState* GetFPSExampleGameState();

	UFUNCTION(BlueprintPure)
	static bool IsServerOrStandalone();

	UFUNCTION(BlueprintPure)
	static UFPSExampleGameInstance* GetFPSExampleGameInstanceWithContext(UObject* contextObject);

	UFUNCTION(BlueprintPure)
	static AFPSExampleGameMode* GetFPSExampleGameModeWithContext(UObject* contextObject);

	UFUNCTION(BlueprintPure)
	static AFPSExampleGameState* GetFPSExampleGameStateWithContext(UObject* contextObject);

	UFUNCTION(BlueprintPure)
	static AFPSExampleCharacter* GetLocalFPSExampleCharacterWithContext(UObject* contextObject);
};