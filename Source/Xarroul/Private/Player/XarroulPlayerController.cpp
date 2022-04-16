// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/XarroulPlayerController.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerSpectatorPawn.h"
#include "Pickups/Pickup.h"
#include "Weapons/Weapon.h"
#include "Pickups/Pickup_PlayerWeapon.h"

void AXarroulPlayerController::PlayerDied(FVector posVal, FRotator rotVal)
{
	//not used atm
	/*ASpectatorPawn* SP = SpawnSpectatorPawn();
	if(SP)
	{
		SP->SetActorLocation(posVal);
		SP->SetActorRotation(rotVal);
		SP->GetMovementComponent()->Deactivate();

		this->Possess(SP);

		ClientIgnoreLookInput(true);
	}*/

}

void AXarroulPlayerController::SpawnWeaponPickupOnDeath(EWeaponType Type, FVector Pos)
{
	//FVector PawnLoc = this->GetPawn()->GetActorLocation();
	//PawnLoc.Z = 0.0f;

	Pos.Z = 0.0f;
	
	TSubclassOf<APickup_PlayerWeapon> PickupToSpawn = nullptr;
	
	switch (Type)
	{
		case Generic:
			PickupToSpawn = WeaponPickup_Generic;
			break;
		case Rapid:
			PickupToSpawn = WeaponPickup_Rapid;
			break;
		case Spread:
			PickupToSpawn = WeaponPickup_Spread;
			break;
		case Homing:
			//TODO
			break;
		case Beam:
			//TODO
			break;
		default: ;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.bNoFail = true;

	FVector RandForceVector = FVector(FMath::RandRange(-100.0f, 100.0f), FMath::RandRange(-100.0f, 100.0f), 0.0f);
	APickup* PU = GetWorld()->SpawnActor<APickup>(PickupToSpawn, Pos + RandForceVector,FRotator::ZeroRotator, SpawnParams);

	//PU->AddImpulseForceToPickup((PU->GetActorLocation() - PawnLoc) * 10.0f); //TODO - add some scatter to pickups.
}
