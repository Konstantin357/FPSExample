// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSExampleGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "FPSExampleCharacter.h"
#include "FPSExample.h"
#include "PickupManager.h"
#include "ScoreManager.h"
#include "FPSExampleGameState.h"
#include "FPSEUtils.h"

AFPSExampleGameMode::AFPSExampleGameMode()
	: Super()
{
	DefaultPawnClass = AFPSExampleCharacter::StaticClass();
}

void AFPSExampleGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PickupManager = NewObject<UPickupManager>();
	ScoreManager = NewObject<UScoreManager>();
}

void AFPSExampleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (ACharacter* character = NewPlayer->GetCharacter())
	{
		RegisterCharacter(character);
	}
	else
	{
		PlayerControllerRegistarionQueue.AddUnique(NewPlayer);
	}
}

void AFPSExampleGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PrematchTimer.IsValid())
	{
		float prematchTime = GetWorldTimerManager().GetTimerRemaining(PrematchTimer);
		if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
		{
			gameState->SetMatchTimeForUI(prematchTime);
		}
	}
	else
	{
		if (ScoreManager->IsValidLowLevel())
		{
			FDateTime matchStartTime = ScoreManager->GetMatchStartTime();
			FDateTime now = FDateTime::UtcNow();
			FTimespan matchTime = now - matchStartTime;
			int32 matchTimeSec = matchTime.GetTotalSeconds();
			if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
			{
				gameState->SetMatchTimeForUI(matchTimeSec);
			}
		}
	}
}

void AFPSExampleGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	for (APlayerController* playerController: PlayerControllerRegistarionQueue)
	{
		if (ACharacter* character = playerController->GetCharacter())
		{
			RegisterCharacter(character);
		}
		else
		{
			UE_LOG(LogFPSExample, Error, TEXT("Could not add character from %s to player register."), *GetNameSafe(playerController));
		}
	}
	PlayerControllerRegistarionQueue.Empty();
}

void AFPSExampleGameMode::RegisterCharacter(ACharacter* inCharacter)
{
	if (AFPSExampleCharacter* FPSExampleCharacter = Cast<AFPSExampleCharacter>(inCharacter))
	{
		if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
		{
			gameState->RegisterCharacter(FPSExampleCharacter);
		}

		if (ScoreManager->IsValidLowLevel())
		{
			ScoreManager->RegisterCharacter(FPSExampleCharacter);
		}

		if (UWorld* world = GetWorld())
		{
			FTransform transform = FPSExampleCharacter->GetTransform();
			if (AFPSExampleWeapon* weapon = world->SpawnActor<AFPSExampleWeapon>(DefaultWeapon, transform))
			{
				FPSExampleCharacter->AttachWeapon(weapon);
			}
		}

		FTimerManager& timerManager = GetWorldTimerManager();
		if (PrematchTimer.IsValid())
		{
			timerManager.ClearTimer(PrematchTimer);
			PrematchTimer.Invalidate();
		}

		timerManager.SetTimer(PrematchTimer, this, &AFPSExampleGameMode::StartFPSExampleMatch, PrematchTime);
	}
}

void AFPSExampleGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSExampleGameMode::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	Super::EndPlay(InEndPlayReason);
}

void AFPSExampleGameMode::StartFPSExampleMatch()
{
	FTimerManager& timerManager = GetWorldTimerManager();
	timerManager.ClearTimer(PrematchTimer);
	PrematchTimer.Invalidate();
	
	if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
	{
		PickupManager->SpawnFirstPickup();
		ScoreManager->OnMatchStarted();

		gameState->SetMatchStarted(true);

		timerManager.SetTimer(TimedDamageTimer, this, &AFPSExampleGameMode::TimedDamageExecute, TimedDamagePeriod, true, TimedDamagePeriod);
	}
}

UPickupManager* AFPSExampleGameMode::GetPickupManager()
{
	return PickupManager;
}

UScoreManager* AFPSExampleGameMode::GetScoreManager()
{
	return ScoreManager;
}

void AFPSExampleGameMode::TimedDamageExecute()
{
	if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
	{
		const TArray<AFPSExampleCharacter*>& registeredCharacters = gameState->GetRegisteredCharacters();
		for (AFPSExampleCharacter* character : registeredCharacters)
		{
			character->AddCurrentHP((-1) * TimedDamageHPDecrement);
		}
	}
}

int32 AFPSExampleGameMode::GetProjectileDamage()
{
	if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
	{
		if (gameState->IsFPSMatchStarted())
		{
			return ProjectileDamage;
		}
	}
	return 0;
}

int32 AFPSExampleGameMode::GetPickupAddedHP()
{
	if (AFPSExampleGameState* gameState = UFPSEUtils::GetFPSExampleGameStateWithContext(this))
	{
		if (gameState->IsFPSMatchStarted())
		{
			return PickupAddedHP;
		}
	}
	return 0;
}