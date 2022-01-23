// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Weapons/Projectile.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	bIsFiring = false;
}

void AWeapon::StartFiring()
{
	if(!bIsFiring)
	{
		bIsFiring = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FireIntervalTimer, this, &AWeapon::Fire, FireInterval, true, FireInterval);
	}
}

void AWeapon::StopFiring()
{
	bIsFiring = false;
	UWorld* World = GetWorld();
	World->GetTimerManager().ClearTimer(FireIntervalTimer);
}

void AWeapon::Fire_Implementation()
{
	UWorld* World = GetWorld();
	if (World)
	{
		FTransform MuzzleTransform = WeaponMesh->GetSocketTransform(FirePointSocketName);
		FVector Dir = MuzzleTransform.GetRotation().GetForwardVector();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn the projectile at the muzzle.
		AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleTransform.GetLocation(), Dir.Rotation(), SpawnParams);
		if (Projectile)
		{
			Projectile->FireInDirection(Projectile->GetActorForwardVector());
		}
	}
}

bool AWeapon::Fire_Validate()
{
	return true;
}

