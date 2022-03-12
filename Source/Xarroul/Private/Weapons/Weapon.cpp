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

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	bIsFiring = false;
}

void AWeapon::ServerFire_Implementation()
{
	if(!bCanFireMultiple || FireRotationOffsets.IsEmpty())
	{
		const AProjectile* Projectile = GetNewProjectile();
		GetNewProjectile()->FireInDirection(Projectile->GetActorForwardVector());
	}
	else
	{
		for (int i = 0; i < FireRotationOffsets.Num(); i++)
		{
			AProjectile* Projectile = GetNewProjectile();
			if(Projectile)
			{
				Projectile->AddActorLocalRotation(FireRotationOffsets[i]);
				FVector _ShootDir = Projectile->GetActorForwardVector();
				
				Projectile->FireInDirection(_ShootDir);
			}
		}
	}
}

AProjectile* AWeapon::GetNewProjectile()
{
	UWorld* World = GetWorld();
	if (World)
	{
		const FTransform MuzzleTransform = WeaponMesh->GetSocketTransform(FirePointSocketName);
		const FVector Dir = MuzzleTransform.GetRotation().GetForwardVector();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();

		return  World->SpawnActor<AProjectile>(ProjectileClass, MuzzleTransform.GetLocation(), Dir.Rotation(), SpawnParams);
		
	}

	return nullptr;
}

void AWeapon::HandleFire()
{
	ServerFire();
}

void AWeapon::StartFiring()
{
	if(!bIsFiring)
	{
		bIsFiring = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FireIntervalTimer, this, &AWeapon::HandleFire, FireInterval, true, FireInterval);
	}
}

void AWeapon::StopFiring()
{
	bIsFiring = false;
	UWorld* World = GetWorld();
	World->GetTimerManager().ClearTimer(FireIntervalTimer);
}


