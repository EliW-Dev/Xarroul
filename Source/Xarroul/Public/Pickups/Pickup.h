// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS(Abstract)
class XARROUL_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "PickupMesh")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Collider")
	USphereComponent* CollisionSphere;

	UPROPERTY(EditDefaultsOnly)
	FRotator RotationDelta;
	
	UPROPERTY(EditDefaultsOnly)
	float RotationSpeed;

	UPROPERTY(EditDefaultsOnly)
	float LifeTime;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(Server, Reliable)
	virtual void ServerOnActorOverlap(class APlayerCharacter* OtherActor);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
