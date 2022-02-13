// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/Weapon.h"
#include "Weapons/WeaponPlacementProxy.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 1200.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	GameCameraHeight = 1200.0f;
	WeaponPlacementCameraHeight = 350.0f;

	bReplicates = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent* Move = GetCharacterMovement();
	if(Move != nullptr)
	{
		Move->MovementMode = EMovementMode::MOVE_Flying;
	}

	this->OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::HandleTakeDamage);

	SpringArmComp->TargetArmLength = GameCameraHeight;
	
	bIsDead = false;
	Health = MaxHealth;

	//reset BP engine effects
	InputThrustValue = 0.0f;
	OnInputThrustValueUpdated();
	InputRotationValue = 0.0f;
	OnInputRotValueUpdated();
	
	//add a starter weapon on spawn
	if(StarterWeapon != nullptr && GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(StarterWeapon, FVector::ZeroVector,
																	  FRotator::ZeroRotator, SpawnParams);
		if (Weapon)
		{
			Weapon->SetOwner(this);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
													WeaponAttachSockets[0]);
		}

		FShipWeaponData WeaponData = FShipWeaponData();
		WeaponData.WeaponType = Weapon;
		WeaponData.WeaponPosition = 0;
		CurrentWeapons.Add(WeaponData);
	}
}

void APlayerCharacter::HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(DamageCauser == DamagedActor || bIsDead == true) return;

	Health -= Damage;

	if(Health <= 0.0f)
	{
		bIsDead = true;
		Destroy(); //TODO - don't just destroy actor...
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("TurnRight", this, &APlayerCharacter::TurnRight);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &APlayerCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &APlayerCharacter::StopFiring);
	PlayerInputComponent->BindAction("WeaponPlacementView", IE_Pressed, this, &APlayerCharacter::ToggleWeaponPlacement);
	PlayerInputComponent->BindAction("WeaponPlacement", IE_Pressed, this, &APlayerCharacter::PlaceNewWeapon);
}

void APlayerCharacter::MoveForward(float Value)
{
	const float tempThrustValue = Value >= 0.0f? Value : Value / 3.0f;
	if(IsLocallyControlled())
	{
		InputThrustValue = tempThrustValue;
		OnInputThrustValueUpdated();
	}
	Server_MoveForward(tempThrustValue);
	
	if (bWeaponPlaceemntViewActive || !Controller || Value == 0.0f) return;
	
	//SetActorLocation(GetActorLocation() + (GetActorForwardVector() * (Value * MoveSpeed)));
	AddMovementInput(GetActorForwardVector(), Value * MoveSpeed);
}

void APlayerCharacter::Server_MoveForward_Implementation(float Value)
{
	InputThrustValue = Value;
	OnInputThrustValueUpdated();
}

void APlayerCharacter::TurnRight(float Value)
{
	if(IsLocallyControlled())
	{
		InputRotationValue = Value;
		OnInputRotValueUpdated();
	}
	Server_TurnRight(Value);
	
	if (bWeaponPlaceemntViewActive || !Controller || Value == 0.0f)  return;
	
	AddControllerYawInput(Value * RotationSpeed * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Server_TurnRight_Implementation(float Value)
{
	InputRotationValue = Value;
	OnInputRotValueUpdated();
}

/*void APlayerCharacter::StartFiring_Implementation()
{
	if(bWeaponPlaceemntViewActive) return;
	
	for(int i = 0; i < CurrentWeapons.Num(); i++)
	{
		AWeapon* Weapon = Cast<AWeapon>(CurrentWeapons[i].WeaponType);
		if(Weapon != nullptr)
		{
			Weapon->StartFiring();
		}
	}
}*/

void APlayerCharacter::StartFiring()
{
	if(bWeaponPlaceemntViewActive) return;
	
	for(int i = 0; i < CurrentWeapons.Num(); i++)
	{
		AWeapon* Weapon = Cast<AWeapon>(CurrentWeapons[i].WeaponType);
		if(Weapon != nullptr)
		{
			Weapon->StartFiring();
		}
	}
}

/*void APlayerCharacter::StopFiring_Implementation()
{
	for(int i = 0; i < CurrentWeapons.Num(); i++)
	{
		AWeapon* Weapon = Cast<AWeapon>(CurrentWeapons[i].WeaponType);
		if(Weapon != nullptr)
		{
			Weapon->StopFiring();
		}
	}
}*/

void APlayerCharacter::StopFiring()
{	
	for(int i = 0; i < CurrentWeapons.Num(); i++)
	{
		AWeapon* Weapon = Cast<AWeapon>(CurrentWeapons[i].WeaponType);
		if(Weapon != nullptr)
		{
			Weapon->StopFiring();
		}
	}
}

void APlayerCharacter::ToggleWeaponPlacement()
{	
	if(!bWeaponPlaceemntViewActive && CollectedWeapon == nullptr) return;
	
	bWeaponPlaceemntViewActive = !bWeaponPlaceemntViewActive;	

	APlayerController* PC = Cast<APlayerController>(GetController());
	if(PC == nullptr) return;
	
	if(bWeaponPlaceemntViewActive)
	{
		WeaponPlacementProxies.Empty();
		
		UCharacterMovementComponent* MoveComp = this->GetCharacterMovement();
		if(MoveComp)
		{
			MoveComp->Velocity = FVector::ZeroVector;
		}
		
		SpringArmComp->TargetArmLength = WeaponPlacementCameraHeight;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		UWorld* World = GetWorld();
		if (World && WeaponPlacementProxy)
		{
			for (int i = 0; i < WeaponAttachSockets.Num(); i++)
			{
				AWeaponPlacementProxy* Proxy = GetWorld()->SpawnActor<AWeaponPlacementProxy>(WeaponPlacementProxy, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
				if (Proxy)
				{
					Proxy->SetOwner(this);
					Proxy->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
															WeaponAttachSockets[i]);

					Proxy->SetPositionID(i);
					WeaponPlacementProxies.Add(Proxy);
				}
			}
		}
	}
	else
	{
		SpringArmComp->TargetArmLength = GameCameraHeight;
		for (int i = 0; i < WeaponPlacementProxies.Num(); i++)
		{
			WeaponPlacementProxies[i]->Destroy();
		}

		WeaponPlacementProxies.Empty();
	}

	PC->bShowMouseCursor = bWeaponPlaceemntViewActive;
	PC->bEnableMouseOverEvents = bWeaponPlaceemntViewActive;
	PC->bEnableClickEvents = bWeaponPlaceemntViewActive;
}

void APlayerCharacter::PlaceNewWeapon()
{	
	if(!bWeaponPlaceemntViewActive || CollectedWeapon == nullptr)
	{
		return;
	}

	FHitResult Hit;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if(PC)
	{
		if(PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, Hit))
		{
			AWeaponPlacementProxy* Proxy = Cast<AWeaponPlacementProxy>(Hit.GetActor());
			if(Proxy)
			{
				int PlacementPos = Proxy->GetPositionID();

				ServerPlaceNewWeapon(PlacementPos);

				/*FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(CollectedWeapon, FVector::ZeroVector,
																  FRotator::ZeroRotator, SpawnParams);
				if (Weapon)
				{
					Weapon->SetOwner(this);
					Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSockets[PlacementPos]);
					
					for (int i = 0; i < CurrentWeapons.Num(); i++)
					{
						if (CurrentWeapons[i].WeaponPosition == PlacementPos)
						{
							CurrentWeapons[i].WeaponType->Destroy();
							CurrentWeapons.RemoveAt(i, 1, true);
							UE_LOG(LogTemp, Warning, TEXT("Weapon Destroyed!"));
							break;
						}
					}

					FShipWeaponData WeaponData = FShipWeaponData();
					WeaponData.WeaponType = Weapon;
					WeaponData.WeaponPosition = PlacementPos;
					CurrentWeapons.Add(WeaponData);

					ToggleWeaponPlacement();
				}*/

				ToggleWeaponPlacement();
			}
		}
	}
}

void APlayerCharacter::ServerPlaceNewWeapon_Implementation(int PlacementPos)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(CollectedWeapon, FVector::ZeroVector,
													  FRotator::ZeroRotator, SpawnParams);
	if (Weapon)
	{
		Weapon->SetOwner(this);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSockets[PlacementPos]);
					
		for (int i = 0; i < CurrentWeapons.Num(); i++)
		{
			if (CurrentWeapons[i].WeaponPosition == PlacementPos)
			{
				CurrentWeapons[i].WeaponType->Destroy();
				CurrentWeapons.RemoveAt(i, 1, true);
				UE_LOG(LogTemp, Warning, TEXT("Weapon Destroyed!"));
				break;
			}
		}

		FShipWeaponData WeaponData = FShipWeaponData();
		WeaponData.WeaponType = Weapon;
		WeaponData.WeaponPosition = PlacementPos;
		CurrentWeapons.Add(WeaponData);

		CollectedWeapon = nullptr;
		//ToggleWeaponPlacement();
	}
	
}

void APlayerCharacter::OnCollectedWeapon(TSubclassOf<AWeapon> Pickup)
{
	CollectedWeapon = Pickup;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerCharacter, CurrentWeapons);
	DOREPLIFETIME(APlayerCharacter, CollectedWeapon);
	DOREPLIFETIME_CONDITION(APlayerCharacter, InputThrustValue, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APlayerCharacter, InputRotationValue, COND_SkipOwner);
}

