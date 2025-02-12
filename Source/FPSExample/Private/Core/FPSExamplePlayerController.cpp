// Copyright Epic Games, Inc. All Rights Reserved.


#include "FPSExamplePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "FPSEUtils.h"
#include "FPSExampleGameState.h"

void AFPSExamplePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AFPSExamplePlayerController::UpdateLocalCharacterPickupScore_Implementation(int32 inPickupScore)
{
	if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
	{
		gameState->SetLocalCharacterPickupScore(inPickupScore);
	}
}

void AFPSExamplePlayerController::UpdateMatchResults_Implementation(const FMatchResult& inMatchResult)
{
	if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
	{
		gameState->ProcessNewMatchresult(inMatchResult);
	}
}