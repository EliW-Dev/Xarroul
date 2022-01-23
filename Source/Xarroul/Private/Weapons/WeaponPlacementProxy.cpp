// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponPlacementProxy.h"

// Sets default values
AWeaponPlacementProxy::AWeaponPlacementProxy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	ProxyMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProxyMesh");
	ProxyMesh->SetupAttachment(RootComponent);
	ProxyMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void AWeaponPlacementProxy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponPlacementProxy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

