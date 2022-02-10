// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float FireInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	FName FirePointSocketName;
	
	FTimerHandle FireIntervalTimer;

	bool bIsFiring;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;
	
	UFUNCTION(Server, Reliable)
	virtual void ServerFire();

	void HandleFire();
	
public:	

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();
};
