// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank_Character.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Projectile.h"

// Sets default values
ATank_Character::ATank_Character()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	// RootComponent = BoxComponent;
	
	// BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	// BaseMesh->SetWorldScale3D(FVector(1.2f, 2, 1));
	// BaseMesh->SetupAttachment(BoxComponent);
	
	// TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	// TurretMesh->SetupAttachment(BaseMesh);
	
	// ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	// ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	MotorParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Mortor Particles System Component"));
	MotorParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATank_Character::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<APlayerController>(GetController());

}

// Called every frame
void ATank_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(TankPlayerController)
	{
		FHitResult HitResult;
		TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		
		// DrawDebugSphere(
		// 	GetWorld(), 
		// 	HitResult.ImpactPoint,
		// 	25, 25, FColor::Red, false, -1);
		
		RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ATank_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank_Character::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveLateral"), this, &ATank_Character::MoveLateral);

	// PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATank_Character::LookUp);
	// PlayerInputComponent->BindAxis(TEXT("LookLateral"), this, &ATank_Character::LookLateral);
	
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookLateral"), this, &APawn::AddControllerYawInput);

    // PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank_Character::Turn);
    // PlayerInputComponent->BindAxis(TEXT("Roll"), this, &ATank::Roll);
    // PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank_Character::Fire);
}

// void ATank_Character::LookLateral(float Axis)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("LateralLook AXIS : %f"), Axis);
// 	APawn::AddControllerYawInput(Axis);
// }

// void ATank_Character::LookUp(float Axis)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("LookUp AXIS : %f"), Axis);
// 	APawn::AddControllerPitchInput(Axis);
// }

void ATank_Character::HandleDestruction()
{
	FVector Scale = FVector(3, 3, 3);
	if(DeathCameraShakeClass && DeathSound && DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation(), Scale);
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
		SetActorHiddenInGame(true);
    	SetActorTickEnabled(false);
		bAlive = false;
	}
}

void ATank_Character::RotateTurret(FVector LookAtTarget)
{
	if(TurretMesh)
	{
		FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	
		FRotator LookAtRotation = ToTarget.Rotation();
		LookAtRotation.Pitch = 0;
		LookAtRotation.Roll = 0;

		TurretMesh->SetWorldRotation(
			FMath::RInterpTo(
				TurretMesh->GetComponentRotation(),
				LookAtRotation,
				UGameplayStatics::GetWorldDeltaSeconds(this),
				5
			)
		);
	}
}

void ATank_Character::Fire()
{
	if(ProjectileSpawnPoint)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		if(Projectile)
		{
			Projectile->SetOwner(this);
		}	
	}
}

void ATank_Character::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ATank_Character::MoveLateral(float Value)
{
	// code from Toon Tank
	// float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	// FVector DeltaLocation;
	// DeltaLocation.Y = Value * DeltaTime * Speed * 2;
	// AddActorLocalOffset(DeltaLocation, true);

	// code from the TPS
	// AddActorWorldOffset(DeltaLocation, true);
	// AddMovementInput(GetActorRightVector() * Value, 1, false);
	
	// UE_LOG(LogTemp, Warning, TEXT("%f"), GetActorRightVector().X);
	AddMovementInput(GetActorRightVector() * Value);
}

void ATank_Character::Turn(float Value)
{
	FRotator currentRoration = GetActorRotation();
	// if(currentRoration.Roll < 7.5 && currentRoration.Roll > -7.5 )
	// {
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		FRotator DeltaRotation(0.f);
		DeltaRotation.Yaw = Value * DeltaTime * TurnRate;
		AddActorLocalRotation(DeltaRotation);
	// }
}

void ATank_Character::Roll(float Value)
{
	FRotator currentRoration = GetActorRotation();
	// UE_LOG(LogTemp, Warning, TEXT("%f"), currentRoration.Roll);
	if (Value == 1 && currentRoration.Roll < 45)
	{
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		FRotator DeltaRotation(0.f);
		DeltaRotation.Roll = Value * DeltaTime * RollRate;
		AddActorLocalRotation(DeltaRotation, true);
		// AddActorLocalRotation(DeltaRotation, true);
	}

	else if (Value == -1 && currentRoration.Roll > -45)
	{
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		FRotator DeltaRotation(0.f);
		DeltaRotation.Roll = Value * DeltaTime * RollRate;
		AddActorLocalRotation(DeltaRotation, true);
	}
	
}