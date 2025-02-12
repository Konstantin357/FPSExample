// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FPSExampleCharacter.h"
#include "FPSExampleProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "FPSExampleWeapon.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UTP_WeaponComponent::Fire(const FRotator inProjectileSpawnRotation)
{
	if (ProjectileClass->IsValidLowLevel())
	{
		if (UWorld* world = GetWorld())
		{
			if (AActor* owner = GetOwner())
			{
				if (AFPSExampleWeapon* ownerWeapon = Cast<AFPSExampleWeapon>(owner))
				{
					const FVector projectileSpawnLocation = ownerWeapon->GetProjectileSpawnLocation();
					// Not using actual muzzle direction because of animation crooked positioning
					//const FRotator projectileSpawnRotation = ownerWeapon->GetProjectileSpawnRotation();
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					world->SpawnActor<AFPSExampleProjectile>(ProjectileClass, projectileSpawnLocation, inProjectileSpawnRotation, ActorSpawnParams);
				}
			}
		}
	}
	
	if (Character->IsValidLowLevel())
	{
		// Try and play the sound if specified
		if (FireSound->IsValidLowLevel())
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}

		// Try and play a firing animation if specified
		if (FireAnimation->IsValidLowLevel())
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			if (AnimInstance->IsValidLowLevel())
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

bool UTP_WeaponComponent::AttachWeapon(AFPSExampleCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}