// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Pickup.h"

#include "Player/PlayerCharacter.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(20.0f);
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Vehicle, ECR_Overlap);
	CollisionSphere->CanCharacterStepUpOn = ECB_No;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PickupMesh->SetupAttachment(RootComponent);
	PickupMesh->SetCollisionProfileName("NoCollision");
	PickupMesh->CanCharacterStepUpOn = ECB_No;
	PickupMesh->SetEnableGravity(false);
	PickupMesh->SetGenerateOverlapEvents(false);

	bReplicates = true;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	//safety check
	if(RotationSpeed == NULL || RotationDelta == FRotator::ZeroRotator)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rotation Data missing for: %s"), *this->GetName());
	}
}

void APickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(OtherActor->GetLocalRole() != ROLE_Authority) return;

	APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor);
	if (PC)
	{
		ServerOnActorOverlap(PC);
		SetLifeSpan(0.3f);
	}	
}

void APickup::ServerOnActorOverlap_Implementation(APlayerCharacter* OtherActor)
{
	//ref child classes
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(GetActorRotation() + (RotationDelta * DeltaTime));
}

