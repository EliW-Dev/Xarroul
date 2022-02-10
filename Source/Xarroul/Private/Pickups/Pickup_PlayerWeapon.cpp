// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Pickup_PlayerWeapon.h"

#include "Player/PlayerCharacter.h"

void APickup_PlayerWeapon::ServerOnActorOverlap(APlayerCharacter* OtherActor)
{
	Super::ServerOnActorOverlap(OtherActor);

	OtherActor->OnCollectedWeapon(PickupWeapon);
}
