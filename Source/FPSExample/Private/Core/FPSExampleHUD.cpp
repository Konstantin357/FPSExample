// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExampleHUD.h"
#include "Blueprint/UserWidget.h"

void AFPSExampleHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainFPSWidget->IsValidLowLevel())
	{
		MainFPSWidget->AddToViewport();
	}
}

void AFPSExampleHUD::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	if (MainFPSWidget->IsValidLowLevel())
	{
		MainFPSWidget->RemoveFromParent();
	}

	Super::EndPlay(InEndPlayReason);
}