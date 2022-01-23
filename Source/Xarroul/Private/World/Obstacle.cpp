// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Obstacle.h"

#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ObstacleMesh->SetupAttachment(RootComponent);

	HitDamage = 10.0f;
	bCanDamagePlayer = true;
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AObstacle::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	APlayerCharacter* PC = Cast<APlayerCharacter>(Other);
	if(PC == nullptr) return;

	UGameplayStatics::ApplyDamage(Other, HitDamage, nullptr, this, nullptr);
}

// Called every frame
void AObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(GetActorRotation() + (RotationDelta * DeltaTime));
}

