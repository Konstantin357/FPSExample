// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExampleJumpPad.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
AFPSExampleJumpPad::AFPSExampleJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	JumpPadRootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("JumpPadRoot"));
	JumpPadRootComponent->bHiddenInGame = true;

	RootComponent = JumpPadRootComponent;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ArrowComponent->SetRelativeRotation(FRotator(45.0, 0.0, 0.0));
	ArrowComponent->bHiddenInGame = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMeshComponent->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AFPSExampleJumpPad::BeginPlay()
{
	Super::BeginPlay();
	
	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSExampleJumpPad::OnJumpPadOverlap);
}

void AFPSExampleJumpPad::EndPlay(const EEndPlayReason::Type InEndPlayReason)
{
	StaticMeshComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AFPSExampleJumpPad::OnJumpPadOverlap);

	Super::EndPlay(InEndPlayReason);
}

// Called every frame
void AFPSExampleJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSExampleJumpPad::OnJumpPadOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor->IsValidLowLevel()) && (OtherActor != this) && (OtherComp->IsValidLowLevel()))
	{
		if (ACharacter* character = Cast<ACharacter>(OtherActor))
		{
			FVector arrowForwardVector = ArrowComponent->GetForwardVector();
			FVector arrowHorizontalDirection = (ArrowComponent->GetForwardVector() * FVector(1.0, 1.0, 0.0)).GetSafeNormal();
			FVector jumpVector = JumpUpSpeed * FVector(0.0, 0.0, 1.0) + JumpSideSpeed * arrowHorizontalDirection;
			character->LaunchCharacter(jumpVector, false, false);
		}
	}
}