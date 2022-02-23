// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	BoxComponent->InitBoxExtent(FVector::OneVector);
	BoxComponent->SetupAttachment(RootComponent);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMesh->SetupAttachment(RootComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComponent->SetUpdatedComponent(RootComponent);	
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = false;
	MovementComponent->Bounciness = 0.0f;
	MovementComponent->ProjectileGravityScale = 0.0f;

	bReplicates = true;
	
	DamageDelt = 20.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime == NULL ? 3.0f : LifeTime);
}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor);
	if(PC)
	{
		UGameplayStatics::ApplyDamage(PC, DamageDelt, nullptr, GetOwner(), DamageType);
	}
	
	//spawn hit effect
	ProjectileMesh->SetVisibility(false);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(0.1f);
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;	
}


