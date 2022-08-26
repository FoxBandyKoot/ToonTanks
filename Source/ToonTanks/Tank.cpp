// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "GameFramework/SpringArmComponent.h" 
#include "Camera/CameraComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h" 
#include "Components/AudioComponent.h" 

ATank::ATank()
{
    PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(RootComponent);

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	MotorParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Mortor Particles System Component"));
	MotorParticles->SetupAttachment(RootComponent);


    int32 musicNumber = FMath::RandRange(1, 10);
    
    static ConstructorHelpers::FObjectFinder<USoundCue> propellerCue(
        TEXT("/Content/Assets/Audio/phantom-116107_Cue")
    );

    // Store a reference to the Cue asset - we'll need it later.
    propellerAudioCue = propellerCue.Object;

    propellerAudioComponent = CreateDefaultSubobject<UAudioComponent>(
        TEXT("PropellerAudioComp")
    );
    // I don't want the sound playing the moment it's created.
    propellerAudioComponent->bAutoActivate = true;
	propellerAudioComponent->SetupAttachment(RootComponent);
    // UGameplayStatics::PlaySound2D(GetWorld(), propellerAudioCue, 1, 1, 0);
}

void ATank::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (propellerAudioCue->IsValidLowLevelFast()) {
		propellerAudioComponent->SetSound(propellerAudioCue);
	}
}


void ATank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
	bAlive = false;
}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	TankPlayerController = Cast<APlayerController>(GetController());
   	propellerAudioComponent->Play();
}

// Called every frame
void ATank::Tick(float DeltaTime)
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
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::MoveForward);
    // PlayerInputComponent->BindAxis(TEXT("MoveLateral"), this, &ATank::MoveLateral);
	// PlayerInputComponent->BindAxis(TEXT("LateralLook"), this, &APawn::AddControllerYawInput);

    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    // PlayerInputComponent->BindAxis(TEXT("Roll"), this, &ATank::Roll);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::MoveForward(float Value)
{
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector DeltaLocation;
	DeltaLocation.X = Value * DeltaTime * Speed;
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::MoveLateral(float Value)
{
	// code from Toon Tank
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	FVector DeltaLocation;
	DeltaLocation.Y = Value * DeltaTime * Speed * 2;
	AddActorLocalOffset(DeltaLocation, true);

	// code from the TPS
	// AddActorWorldOffset(DeltaLocation, true);
	// AddMovementInput(GetActorRightVector() * Value, 1, false);
	// AddMovementInput(GetActorRightVector() * Value, 1, true);
}

void ATank::Turn(float Value)
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

void ATank::Roll(float Value)
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