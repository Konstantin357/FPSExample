// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PickupManager.generated.h"

class AManagedActorSpawner;
class AFPSExampleCharacter;
class UTP_PickupComponent;

/**
 * 
 */
UCLASS()
class FPSEXAMPLE_API UPickupManager : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
	
private:
	UPROPERTY()
	TSet<AManagedActorSpawner*> ActorSpawners;
public:
	void RegisterActorSpawner(AManagedActorSpawner* inActorSpawner);
	void UnregisterActorSpawner(AManagedActorSpawner* inActorSpawner);

private:
	UFUNCTION()
	void OnItemPickedUp(UPickupComponentHP* Pickup, AFPSExampleCharacter* PickupCharacter);
	AManagedActorSpawner* LastSpawner = nullptr;
	bool bFirstPickupSpawned = false;
	
	void SpawnPickup();
public:
	void RegisterPickup(UPickupComponentHP* Pickup);
	void UnregisterPickup(UPickupComponentHP* Pickup);
	void SpawnFirstPickup();
};