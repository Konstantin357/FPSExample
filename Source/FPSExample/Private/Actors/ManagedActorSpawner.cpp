// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagedActorSpawner.h"
#include "Components/SphereComponent.h"
#include "FPSEUtils.h"
#include "PickupManager.h"
#include "FPSExampleGameMode.h"

// Sets default values
AManagedActorSpawner::AManagedActorSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USphereComponent>("SphereRoot");
	RootComponent->bHiddenInGame = true;
}

// Called when the game starts or when spawned
void AManagedActorSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (AFPSExampleGameMode* gameMode = UFPSEUtils::GetFPSExampleGameModeWithContext(this))
	{
		if (UPickupManager* PickupManager = gameMode->GetPickupManager())
		{
			PickupManager->RegisterActorSpawner(this);
		}
	}
}

void AManagedActorSpawner::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	if (AFPSExampleGameMode* gameMode = UFPSEUtils::GetFPSExampleGameModeWithContext(this))
	{
		if (UPickupManager* PickupManager = gameMode->GetPickupManager())
		{
			PickupManager->UnregisterActorSpawner(this);
		}
	}

	Super::EndPlay(InEndPlayReason);
}

// Called every frame
void AManagedActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AManagedActorSpawner::SpawnPredefinedActor() const
{
	if (UWorld* world = GetWorld())
	{
		FTransform transform = GetTransform();
		world->SpawnActor(ActorToSpawn, &transform);
	}
}