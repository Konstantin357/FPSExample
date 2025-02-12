// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FPSExampleCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class AFPSExampleWeapon;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, AFPSExampleCharacter*, Character, int32, NewHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, AFPSExampleCharacter*, Character);

UCLASS(config=Game)
class AFPSExampleCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	
public:
	AFPSExampleCharacter();

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type InEndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void TryFire();
	void FireWeapon(UTP_WeaponComponent* inWeaponComponent, const FRotator inProjectileSpawnRotation);
	UFUNCTION(Server, Reliable)
	void ServerFireWeapon(UTP_WeaponComponent* inWeaponComponent, const FRotator inProjectileSpawnRotation);
public:
	void AttachWeapon(AFPSExampleWeapon* inWeapon);
	UTP_WeaponComponent* GetAttachedWeaponComponent();

private:
	UFUNCTION(Server, Reliable)
	void ServerUpdateCameraComponentRoration(const FRotator inCameraComponentRotation);
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastUpdateCameraComponentRoration(const FRotator inCameraComponentRotation);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	int32 MaxHP = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	int32 CurrentHP = MaxHP;
	UPROPERTY(BlueprintReadOnly, Replicated, Category = Attributes, meta = (AllowPrivateAccess = "true"))
	bool IsAlive = true;
public:
	UFUNCTION(BlueprintPure, Category = Attributes)
	int32 GetMaxHP() const;
	UFUNCTION(BlueprintPure, Category = Attributes)
	int32 GetCurrentHP() const;
	UFUNCTION(BlueprintCallable, Category = Attributes)
	void SetCurrentHP(int32 inHP);
	UFUNCTION(BlueprintCallable, Category = Attributes)
	void AddCurrentHP(int32 inHP);
private:
	void CheckStatus();

	UFUNCTION(Client, Reliable)
	void SetInputsActive(bool inActive);

public:
	UPROPERTY(BlueprintAssignable)
	FOnHPChanged OnHPChanged;
	UPROPERTY(BlueprintAssignable)
	FOnCharacterDied OnCharacterDied;
};