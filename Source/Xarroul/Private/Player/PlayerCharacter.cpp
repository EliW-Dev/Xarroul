// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 800.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	MoveSpeed = 10.0f;
	RotationSpeed = 10.0f;
	
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
}

void APlayerCharacter::MoveForward(float Value)
{
	if (!Controller || Value == 0.0f) {  return; }
	
	//SetActorLocation(GetActorLocation() + (GetActorForwardVector() * (Value * MoveSpeed)));
	AddMovementInput(GetActorForwardVector(), Value * MoveSpeed);
}

void APlayerCharacter::TurnRight(float Value)
{	
	AddControllerYawInput(Value * RotationSpeed * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::StartFiring()
{
}

void APlayerCharacter::StopFiring()
{
}

void APlayerCharacter::ToggleWeaponPlacement()
{
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

