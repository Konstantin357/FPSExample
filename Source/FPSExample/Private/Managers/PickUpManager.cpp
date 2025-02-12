// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupManager.h"
#include "PickupComponentHP.h"
#include "ManagedActorSpawner.h"

void UPickupManager::PostInitProperties()
{
	Super::PostInitProperties();
}

void UPickupManager::RegisterActorSpawner(AManagedActorSpawner* inActorSpawner)
{
	ActorSpawners.Add(inActorSpawner);
}

void UPickupManager::UnregisterActorSpawner(AManagedActorSpawner* inActorSpawner)
{
	ActorSpawners.Remove(inActorSpawner);
}

void UPickupManager::OnItemPickedUp(UPickupComponentHP* Pickup, AFPSExampleCharacter* PickupCharacter)
{
	Pickup->OnPickup.RemoveDynamic(this, &UPickupManager::OnItemPickedUp);

	SpawnPickup();
}

void UPickupManager::RegisterPickup(UPickupComponentHP* Pickup)
{
	Pickup->OnPickup.AddDynamic(this, &UPickupManager::OnItemPickedUp);
}

void UPickupManager::UnregisterPickup(UPickupComponentHP* Pickup)
{
	Pickup->OnPickup.RemoveDynamic(this, &UPickupManager::OnItemPickedUp);
}

void UPickupManager::SpawnFirstPickup()
{
	if (bFirstPickupSpawned == false)
	{
		bFirstPickupSpawned = true;
		SpawnPickup();
	}
}

void UPickupManager::SpawnPickup()
{
	TSet<AManagedActorSpawner*> availableActorSpawners = ActorSpawners;
	if (LastSpawner->IsValidLowLevel() && availableActorSpawners.Num() > 1)
	{
		availableActorSpawners.Remove(LastSpawner);
	}
	int32 randomSpawnerIndex = FMath::RandRange(0, availableActorSpawners.Num() - 1);
	for (AManagedActorSpawner*& spawner : availableActorSpawners)
	{
		if (randomSpawnerIndex == 0)
		{
			LastSpawner = spawner;
			spawner->SpawnPredefinedActor();
			break;
		}
		--randomSpawnerIndex;
	}
}