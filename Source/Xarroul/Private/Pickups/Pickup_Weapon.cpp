// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/Pickup_Weapon.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"

// Sets default values
APickup_Weapon::APickup_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create root
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	// Create Static Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionBox->SetBoxExtent(FVector(32.f,32.f,32.f));
	CollisionBox->SetCollisionProfileName("Trigger");

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APickup_Weapon::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APickup_Weapon::OnOverlapEnd);


	this->RotRate = FRotator(0.0f,180.0f,0.0f);
	this->RotSpeed = 1.f;
	this->MovSpeed = 250.f;

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

	//Movement
	FVector Location = GetActorLocation();
	Location += GetActorForwardVector() * MovSpeed * DeltaTime;
	SetActorLocation(Location);
	//Spin
	this->AddActorLocalRotation(this->RotRate * RotSpeed * DeltaTime);
	
}

void APickup_Weapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("This triggered pickup: %s"), *OtherActor->GetName());
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, "Overlap Begin");
	}
}

void APickup_Weapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("This exited pickup: %s"), *OtherActor->GetName());
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Red, "Overlap End");
	}
}

