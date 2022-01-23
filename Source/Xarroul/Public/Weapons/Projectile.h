// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class XARROUL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float DamageDelt;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool bIsHoming;

	void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float LifeTime;
	
public:	

	void FireInDirection(const FVector& ShootDirection);
};
