// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"

#include "Player/PlayerCharacter.h"
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
	LastFireTime = -FireInterval;

	this->OnTakeAnyDamage.AddDynamic(this, &AWeapon::HandleTakeDamage);
}

void AWeapon::ServerFire_Implementation()
{
	if(bCanFireMultiple && !FireRotationOffsets.IsEmpty())
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
		
		return;
	}

	AProjectile* Projectile = GetNewProjectile();
	if(Projectile)
	{
		Projectile->FireInDirection(Projectile->GetActorForwardVector());
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
	LastFireTime = GetWorld()->TimeSeconds;
}

void AWeapon::HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(DamageCauser == DamagedActor) return;

	Health -= Damage;

	if(Health <= 0.0f)
	{
		APlayerCharacter* PC = Cast<APlayerCharacter>(GetOwner());
		if(PC)
		{
			PC->RomoveWeapon(this);
		}
	}
}

void AWeapon::StartFiring()
{
	if(!bIsFiring)
	{
		bIsFiring = true;
		UWorld* World = GetWorld();
		if(World)
		{
			const float FirstFireDelay = FMath::Max(0.0f, LastFireTime + FireInterval - GetWorld()->TimeSeconds);
			GetWorldTimerManager().SetTimer(FireIntervalTimer, this, &AWeapon::HandleFire, FireInterval, true, FirstFireDelay);
		}
	}
}

void AWeapon::StopFiring()
{
	bIsFiring = false;
	GetWorldTimerManager().ClearTimer(FireIntervalTimer);
}


