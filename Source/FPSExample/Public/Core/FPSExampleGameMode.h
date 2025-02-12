// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSExampleWeapon.h"
#include "FPSExampleGameMode.generated.h"

class UPickupManager;
class UScoreManager;

UCLASS(minimalapi)
class AFPSExampleGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AFPSExampleGameMode();

public:
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void HandleMatchHasStarted() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type InEndPlayReason) override;

private:
	UPROPERTY()
	TArray<APlayerController*> PlayerControllerRegistarionQueue;

	void RegisterCharacter(ACharacter* inCharacter);

	FTimerHandle PrematchTimer;
	void StartFPSExampleMatch();

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPickupManager* PickupManager = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UScoreManager* ScoreManager = nullptr;

public:
	UPickupManager* GetPickupManager();
	UScoreManager* GetScoreManager();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	float PrematchTime = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	int32 ProjectileDamage = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	int32 PickupAddedHP = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	float TimedDamagePeriod = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	int32 TimedDamageHPDecrement = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Match, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFPSExampleWeapon> DefaultWeapon = AFPSExampleWeapon::StaticClass();

	FTimerHandle TimedDamageTimer;
	void TimedDamageExecute();
public:
	int32 GetProjectileDamage();
	int32 GetPickupAddedHP();

private:
	/*UFUNCTION()
	void UpdateGameState();*/
};