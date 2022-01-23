// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pickups/Pickup.h"
#include "Weapons/Weapon.h"
#include "PlayerCharacter.generated.h"

class AWeapon;
class AWeaponPlacementProxy;

USTRUCT()
struct FShipWeaponData
{
	GENERATED_BODY()

	UPROPERTY()
	AWeapon* WeaponType;

	UPROPERTY()
	int WeaponPosition;

	FShipWeaponData()
	{
		WeaponType = nullptr;
		WeaponPosition = 0;
	}
};

UCLASS()
class XARROUL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* CameraComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float RotationSpeed;

	//health / damage
	UFUNCTION()
	void HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY()
	bool bIsDead;

	//weapon test
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> StarterWeapon;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AWeapon> CollectedWeapon;
		
	UPROPERTY(EditDefaultsOnly)
	TArray<FShipWeaponData> CurrentWeapons;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> WeaponAttachSockets;

	bool bWeaponPlaceemntViewActive = false;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float GameCameraHeight;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float WeaponPlacementCameraHeight;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class AWeaponPlacementProxy> WeaponPlacementProxy;

	TArray<AWeaponPlacementProxy*> WeaponPlacementProxies;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void TurnRight(float Value);
	void StartFiring();
	void StopFiring();
	void ToggleWeaponPlacement();
	void PlaceNewWeapon();

	UFUNCTION()
	void OnCollectedWeapon(TSubclassOf<AWeapon> Pickup);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
