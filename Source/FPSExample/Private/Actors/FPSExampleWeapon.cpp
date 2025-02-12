// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExampleWeapon.h"

// Sets default values
AFPSExampleWeapon::AFPSExampleWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSExampleWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSExampleWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AFPSExampleWeapon::GetProjectileSpawnLocation_Implementation()
{
	return GetActorLocation();
}

FRotator AFPSExampleWeapon::GetProjectileSpawnRotation_Implementation()
{
	return GetActorRotation();
}