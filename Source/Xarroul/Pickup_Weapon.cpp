// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Weapon.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Sets default values
APickup_Weapon::APickup_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetBoxExtent(FVector(32.f,32.f,32.f));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APickup_Weapon::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APickup_Weapon::OnOverlapEnd);

}

// Called when the game starts or when spawned
void APickup_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup_Weapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, "Overlap Begin");
}

void APickup_Weapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Red, "Overlap End");
}

