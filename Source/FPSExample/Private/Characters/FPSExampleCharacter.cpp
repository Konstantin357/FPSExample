// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSExampleCharacter.h"
#include "FPSExampleProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "FPSEUtils.h"
#include "Net/UnrealNetwork.h"
#include "TP_WeaponComponent.h"
#include "FPSExamplePlayerController.h"
#include "FPSExampleWeapon.h"
#include "TP_PickUpComponent.h"
#include "ScoreManager.h"
#include "FPSExampleGameMode.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFPSExampleCharacter

AFPSExampleCharacter::AFPSExampleCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AFPSExampleCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AFPSExampleCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	CheckStatus();
}

void AFPSExampleCharacter::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	if (AFPSExampleGameMode* gameMode = UFPSEUtils::GetFPSExampleGameModeWithContext(this))
	{
		if (UScoreManager* scoreManager = gameMode->GetScoreManager())
		{
			scoreManager->UnregisterCharacter(this);
		}
	}

	Super::EndPlay(InEndPlayReason);
}

void AFPSExampleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSExampleCharacter, CurrentHP);
	DOREPLIFETIME(AFPSExampleCharacter, IsAlive);
}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSExampleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSExampleCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSExampleCharacter::Look);

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetController())->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AFPSExampleCharacter::TryFire);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFPSExampleCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller->IsValidLowLevel())
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFPSExampleCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (AController* controller = GetController())
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);

		if (APlayerController* playerController = Cast<APlayerController>(controller))
		{
			if (playerController->PlayerCameraManager->IsValidLowLevel())
			{
				const FRotator cameraComponentRotation = playerController->PlayerCameraManager->GetCameraRotation();
				if (!UFPSEUtils::IsServerOrStandalone())
				{
					ServerUpdateCameraComponentRoration(cameraComponentRotation);
				}
				else
				{
					NetMulticastUpdateCameraComponentRoration(cameraComponentRotation);
				}
			}
		}
	}
}

void AFPSExampleCharacter::TryFire()
{
	if (UTP_WeaponComponent* weaponComponent = GetAttachedWeaponComponent())
	{
		if (AController* controller = GetController())
		{
			if (APlayerController* playerController = Cast<APlayerController>(controller))
			{
				if (playerController->PlayerCameraManager->IsValidLowLevel())
				{
					const FRotator projectileSpawnRotation = playerController->PlayerCameraManager->GetCameraRotation();
					if (UFPSEUtils::IsServerOrStandalone())
					{
						FireWeapon(weaponComponent, projectileSpawnRotation);
					}
					else
					{
						ServerFireWeapon(weaponComponent, projectileSpawnRotation);
					}
				}
			}
		}
	}
}

void AFPSExampleCharacter::FireWeapon(UTP_WeaponComponent* inWeaponComponent, const FRotator inProjectileSpawnRotation)
{
	inWeaponComponent->Fire(inProjectileSpawnRotation);
}

void AFPSExampleCharacter::AttachWeapon(AFPSExampleWeapon* inWeapon)
{
	if (inWeapon->IsValidLowLevel())
	{
		if (UTP_PickUpComponent* pickUpComponent = inWeapon->GetComponentByClass<UTP_PickUpComponent>())
		{
			pickUpComponent->ManualAttachToCharacter(this);
		}
	}
}

UTP_WeaponComponent* AFPSExampleCharacter::GetAttachedWeaponComponent()
{
	const TArray<UActorComponent*>& components = GetInstanceComponents();
	UTP_WeaponComponent* weaponComponentFound = nullptr;
	components.FindItemByClass<UTP_WeaponComponent>(&weaponComponentFound);
	return weaponComponentFound;
}

void AFPSExampleCharacter::ServerUpdateCameraComponentRoration_Implementation(const FRotator inCameraComponentRotation)
{
	FirstPersonCameraComponent->SetWorldRotation(inCameraComponentRotation);

	NetMulticastUpdateCameraComponentRoration(inCameraComponentRotation);
}

void AFPSExampleCharacter::NetMulticastUpdateCameraComponentRoration_Implementation(const FRotator inCameraComponentRotation)
{
	FirstPersonCameraComponent->SetWorldRotation(inCameraComponentRotation);
}

void AFPSExampleCharacter::ServerFireWeapon_Implementation(UTP_WeaponComponent* inWeaponComponent, const FRotator inProjectileSpawnRotation)
{
	FireWeapon(inWeaponComponent, inProjectileSpawnRotation);
}

int32 AFPSExampleCharacter::GetMaxHP() const
{
	return MaxHP;
}

int32 AFPSExampleCharacter::GetCurrentHP() const
{
	return CurrentHP;
}

void AFPSExampleCharacter::SetCurrentHP(int32 inHP)
{
	if (!IsAlive)
		return;

	CurrentHP = inHP;
	CheckStatus();
	OnHPChanged.Broadcast(this, CurrentHP);
}

void AFPSExampleCharacter::AddCurrentHP(int32 inHP)
{
	if (!IsAlive)
		return;

	CurrentHP += inHP;
	CheckStatus();
	OnHPChanged.Broadcast(this, CurrentHP);
}

void AFPSExampleCharacter::CheckStatus()
{
	CurrentHP = FMath::Clamp(CurrentHP, 0, MaxHP);
	if (CurrentHP == 0 && IsAlive == true)
	{
		SetInputsActive(false);
		IsAlive = false;
		OnCharacterDied.Broadcast(this);
	}
}

void AFPSExampleCharacter::SetInputsActive_Implementation(bool inActive)
{
	if (AController* controller = GetController())
	{
		if (AFPSExamplePlayerController* playerController = Cast<AFPSExamplePlayerController>(controller))
		{
			if (ULocalPlayer* localPlayer = playerController->GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer))
				{
					if (UInputMappingContext* inputMappingContext = playerController->GetInputMappingContext())
					{
						if (inActive)
						{
							subsystem->AddMappingContext(FireMappingContext, 1);
							subsystem->AddMappingContext(inputMappingContext, 0);
						}
						else
						{
							subsystem->RemoveMappingContext(FireMappingContext);
							subsystem->RemoveMappingContext(inputMappingContext);
						}
					}
				}
			}
		}
	}
}