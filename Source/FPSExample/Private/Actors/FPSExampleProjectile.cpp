// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSExampleProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "FPSExampleCharacter.h"
#include "FPSExampleGameMode.h"
#include "FPSEUtils.h"

AFPSExampleProjectile::AFPSExampleProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AFPSExampleProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSExampleProjectile::OnHit);
}

void AFPSExampleProjectile::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	CollisionComp->OnComponentHit.RemoveDynamic(this, &AFPSExampleProjectile::OnHit);

	Super::EndPlay(InEndPlayReason);
}

void AFPSExampleProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor->IsValidLowLevel()) && (OtherActor != this) && (OtherComp->IsValidLowLevel()))
	{
		if (OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			Destroy();
		}
		else if (AFPSExampleCharacter* character = Cast<AFPSExampleCharacter>(OtherActor))
		{
			if (AFPSExampleGameMode* gameMode = UFPSEUtils::GetFPSExampleGameModeWithContext(this))
			{
				character->AddCurrentHP((-1) * (gameMode->GetProjectileDamage()));
				Destroy();
			}
		}
	}
}