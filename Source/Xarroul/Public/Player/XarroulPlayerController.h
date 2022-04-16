// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "XarroulPlayerController.generated.h"

/**
 * 
 */

enum EWeaponType : int;
class APickup_PlayerWeapon;

UCLASS()
class XARROUL_API AXarroulPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APickup_PlayerWeapon> WeaponPickup_Generic;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APickup_PlayerWeapon> WeaponPickup_Rapid;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APickup_PlayerWeapon> WeaponPickup_Spread;

	
public:
	
	UFUNCTION()
	void PlayerDied(FVector posVal, FRotator rotVal);

	void SpawnWeaponPickupOnDeath(EWeaponType TypeIndex, FVector Pos);
};
