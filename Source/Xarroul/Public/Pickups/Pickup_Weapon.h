// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup_Weapon.generated.h"

UCLASS()
class XARROUL_API APickup_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup_Weapon();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MoveActor")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "MoveActor")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent * CollisionBox;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	FRotator Spin;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")

	FVector Bob;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")

	float BobSpeedMultiplier;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float MovSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	float RotSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Settings")
	FRotator RotRate;
};
