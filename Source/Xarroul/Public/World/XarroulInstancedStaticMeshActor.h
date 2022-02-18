// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XarroulInstancedStaticMeshActor.generated.h"

USTRUCT()
struct FBackgroundMeshData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int InstanceCountXY;
	
	UPROPERTY(EditDefaultsOnly)
	float InstanceSpacing;

	UPROPERTY(EditDefaultsOnly)
	float DistanceZ;
	
	UPROPERTY(EditDefaultsOnly)
	float MinScaleMultiplier;

	UPROPERTY(EditDefaultsOnly)
	float MaxScaleMultiplier;

	FBackgroundMeshData()
	{
		InstanceCountXY = 0.0f;
		InstanceSpacing = 0.0f;
		DistanceZ = 0.0f;
		MinScaleMultiplier = 0.7f;
		MaxScaleMultiplier = 1.3f;
	}
};

UCLASS()
class XARROUL_API AXarroulInstancedStaticMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXarroulInstancedStaticMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UInstancedStaticMeshComponent* InstancedStaticMeshCube;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UInstancedStaticMeshComponent* InstancedStaticMeshOcta;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UInstancedStaticMeshComponent* InstancedStaticMeshTetra;

	UFUNCTION(CallInEditor)
	void SetupBackgroundMeshs();

	UFUNCTION(CallInEditor)
	void ClearAllBackgroundMeshs();

	UPROPERTY(EditAnywhere, Category = "SpawnData")
	TArray<FBackgroundMeshData> MeshData;
	
	UPROPERTY(EditDefaultsOnly, Category = "Mesh Data")
	int MeshCountXY;
	
	UPROPERTY(EditDefaultsOnly, Category = "Mesh Data")
	float MinDistanceBetweenInstances = 300.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh Data")
	float MaxDistanceBetweenInstances = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh Data")
	int NumberOfLayers = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh Data")
	float MinRandomSize = 0.7f;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh Data")
	float MaxRandomSize = 1.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh Data")
	float BackgroundZOffset;
};
