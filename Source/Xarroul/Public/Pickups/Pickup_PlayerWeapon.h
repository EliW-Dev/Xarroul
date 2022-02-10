// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/Pickup.h"
#include "Pickup_PlayerWeapon.generated.h"

/**
 * 
 */
UCLASS()
class XARROUL_API APickup_PlayerWeapon : public APickup
{
	GENERATED_BODY()

protected:

	virtual void ServerOnActorOverlap(class APlayerCharacter* OtherActor) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeapon> PickupWeapon;
};
