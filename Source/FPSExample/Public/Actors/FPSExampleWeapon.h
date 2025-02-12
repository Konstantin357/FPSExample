// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExampleWeapon.generated.h"

UCLASS()
class FPSEXAMPLE_API AFPSExampleWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSExampleWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Shot parameters
	// Should absolutely be overriden in BP according to weapon mesh
public:
	UFUNCTION(BlueprintNativeEvent)
	FVector GetProjectileSpawnLocation();
	UFUNCTION(BlueprintNativeEvent)
	FRotator GetProjectileSpawnRotation();
};