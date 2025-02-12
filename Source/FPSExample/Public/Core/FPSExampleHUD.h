// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSExampleHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class FPSEXAMPLE_API AFPSExampleHUD : public AHUD
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UUserWidget* MainFPSWidget = nullptr;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type InEndPlayReason) override;
};
