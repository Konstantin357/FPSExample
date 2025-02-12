// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSExamplePlayerController.generated.h"

class UInputMappingContext;
struct FMatchResult;

/**
 *
 */
UCLASS()
class FPSEXAMPLE_API AFPSExamplePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface
public:
	UInputMappingContext* GetInputMappingContext()
	{
		return InputMappingContext;
	}

	UFUNCTION(Client, Reliable)
	void UpdateMatchResults(const FMatchResult& inMatchResult);
	UFUNCTION(Client, Reliable)
	void UpdateLocalCharacterPickupScore(int32 inPickupScore);
};
