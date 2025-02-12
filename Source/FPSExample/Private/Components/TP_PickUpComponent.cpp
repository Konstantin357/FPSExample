// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent.h"
#include "FPSExampleCharacter.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

void UTP_PickUpComponent::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	OnComponentBeginOverlap.RemoveDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);

	Super::EndPlay(InEndPlayReason);
}

void UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AFPSExampleCharacter* character = Cast<AFPSExampleCharacter>(OtherActor))
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}

void UTP_PickUpComponent::ManualAttachToCharacter(AFPSExampleCharacter* inCharacter)
{
	if (inCharacter->IsValidLowLevel())
	{
		OnPickUp.Broadcast(inCharacter);
		OnComponentBeginOverlap.RemoveAll(this);
	}
}