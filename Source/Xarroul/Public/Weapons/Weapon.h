// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM()
enum EWeaponType
{
	Generic     UMETA(DisplayName = "Generic Weapon"),
	Rapid     UMETA(DisplayName = "Rapid Weapon"),
	Spread     UMETA(DisplayName = "Spread Weapon"),
	Homing     UMETA(DisplayName = "Homing Weapon"),
	Beam     UMETA(DisplayName = "Beam Weapon"),
};

UCLASS()
class XARROUL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UBoxComponent* CollisionBox;

	UPROPERTY(EditDefaultsOnly, Category = "Type")
	TEnumAsByte<EWeaponType> TypeOfWeapon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float FireInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	FName FirePointSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	bool bCanFireMultiple;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	TArray<FRotator> FireRotationOffsets;

	class AProjectile* GetNewProjectile();
	
	FTimerHandle FireIntervalTimer;

	bool bIsFiring;

	float LastFireTime;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;
	
	UFUNCTION(Server, Reliable)
	virtual void ServerFire();

	void HandleFire();

	//damage
	UFUNCTION()
	void HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float Health;
	
public:	

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();

	EWeaponType GetWeaponType() {return TypeOfWeapon; }
};
