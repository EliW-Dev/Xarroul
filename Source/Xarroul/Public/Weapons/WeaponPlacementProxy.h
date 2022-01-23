// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../Plugins/Developer/RiderLink/Source/RD/thirdparty/spdlog/include/spdlog/fmt/bundled/core.h"
#include "GameFramework/Actor.h"
#include "WeaponPlacementProxy.generated.h"

UCLASS()
class XARROUL_API AWeaponPlacementProxy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPlacementProxy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ProxyMesh;

	UPROPERTY()
	int PositionId;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetPositionID(float Value) { PositionId = Value; }

	UFUNCTION()
	int GetPositionID() { return PositionId; };
};
