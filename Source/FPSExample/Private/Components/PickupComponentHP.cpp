// Copyright Epic Games, Inc. All Rights Reserved.

#include "PickupComponentHP.h"
#include "FPSEUtils.h"
#include "FPSExampleGameMode.h"
#include "PickupManager.h"
#include "FPSExampleCharacter.h"
#include "ScoreManager.h"

UPickupComponentHP::UPickupComponentHP()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UPickupComponentHP::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UPickupComponentHP::OnSphereBeginOverlap);

	if (AFPSExampleGameMode* gameMode = UFPSEUtils::GetFPSExampleGameModeWithContext(this))
	{
		if (UPickupManager* pickupManager = gameMode->GetPickupManager())
		{
			pickupManager->RegisterPickup(this);
		}

		if (UScoreManager* scoreManager = gameMode->GetScoreManager())
		{
			scoreManager->RegisterPickup(this);
		}
	}
}

void UPickupComponentHP::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	OnComponentBeginOverlap.RemoveDynamic(this, &UPickupComponentHP::OnSphereBeginOverlap);

	if (AFPSExampleGameMode* gameMode = UFPSEUtils::GetFPSExampleGameModeWithContext(this))
	{
		if (UPickupManager* pickupManager = gameMode->GetPickupManager())
		{
			pickupManager->UnregisterPickup(this);
		}

		if (UScoreManager* scoreManager = gameMode->GetScoreManager())
		{
			scoreManager->UnregisterPickup(this);
		}
	}

	Super::EndPlay(InEndPlayReason);
}

void UPickupComponentHP::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AFPSExampleCharacter* character = Cast<AFPSExampleCharacter>(OtherActor))
	{
		if (AActor* owner = GetOwner())
		{
			if (AFPSExampleGameMode* gameMode = UFPSEUtils::GetFPSExampleGameModeWithContext(this))
			{
				character->AddCurrentHP(gameMode->GetPickupAddedHP());
				OnPickup.Broadcast(this, character);
				owner->Destroy();
			}
		}
	}
}