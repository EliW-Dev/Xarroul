// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Pickup_PlayerWeapon.h"

#include "Player/PlayerCharacter.h"

void APickup_PlayerWeapon::OnActorOverlap(AActor* OtherActor)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor);
	if(PC == nullptr) return;

	PC->OnCollectedWeapon(PickupWeapon);
}
