// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class XARROUL_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ObstacleMesh;
	
	UPROPERTY(EditDefaultsOnly)
	bool bCanDamagePlayer;

	UPROPERTY(EditDefaultsOnly)
	FRotator RotationDelta;

	UPROPERTY(EditDefaultsOnly)
	float HitDamage;

	FTimerHandle DamageTimer;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
