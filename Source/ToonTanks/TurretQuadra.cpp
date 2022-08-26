// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretQuadra.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Tower.h"
#include "Projectile.h"
#include "HealthComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" 
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h" 

ATurretQuadra::ATurretQuadra()
{
    TurretMeshLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh Left"));
	TurretMeshLeft->SetupAttachment(TurretMesh);

    TurretMeshRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh Right"));
	TurretMeshRight->SetupAttachment(TurretMesh);

    TurretMeshTop = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh Top"));
	TurretMeshTop->SetupAttachment(TurretMesh);

    ProjectileSpawnPointLeft = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point Left"));
	ProjectileSpawnPointLeft->SetupAttachment(TurretMeshLeft);

    ProjectileSpawnPointRight = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point Right"));
	ProjectileSpawnPointRight->SetupAttachment(TurretMeshRight);

    ProjectileSpawnPointTop = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point Top"));
	ProjectileSpawnPointTop->SetupAttachment(TurretMeshTop);
}

void ATurretQuadra::BeginPlay()
{
    Super::BeginPlay();
    this->OnTakeAnyDamage.AddDynamic(this, &ATurretQuadra::DamageTaken);
    GetWorldTimerManager().SetTimer(CheckRangeTimerHandle, this, &ATurretQuadra::CheckFireConditions, FireRate, true);
}

// Called every frame
void ATurretQuadra::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(Tank && CheckRangeConditions())
    {
        RotateTurret(Tank->GetActorLocation());
    }
}

bool ATurretQuadra::CheckRangeConditions()
{
    return Super::CheckRangeConditions();
}

void ATurretQuadra::RotateTurret(FVector LookAtTarget)
{
    FVector TurretLocation = GetActorLocation();
    double X = TurretLocation.X - LookAtTarget.X;
    double Y = TurretLocation.Y - LookAtTarget.Y;

    double distToTarget = X * X + Y * Y;
    UE_LOG(LogTemp, Warning, TEXT("distToTarget : %f"), distToTarget);


	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	
	FRotator LookAtRotation = ToTarget.Rotation();
    UE_LOG(LogTemp, Warning, TEXT("LookAtTarget.X : %f"), LookAtTarget.X);
    // LookAtRotation.Pitch = LookAtTarget.X / 90 * -1 ; // -1 is to reverse the orientation, less the target is close, more the turret needs to look away
    // UE_LOG(LogTemp, Warning, TEXT("LookAtRotation.Pitch : %f"), LookAtRotation.Pitch);

    const double ZeroPitchImpact = 14041942.331520;
    // LookAtRotation.Pitch = 0;
    double pitchOrientation = 1;
    if(distToTarget - ZeroPitchImpact < 0)
    {
        pitchOrientation = -1;
    }
    LookAtRotation.Pitch = 21 * pitchOrientation * FMath::Square((distToTarget - ZeroPitchImpact) / ZeroPitchImpact);

    // if(LookAtRotation.Pitch > 0){LookAtRotation.Pitch = 0;}
    UE_LOG(LogTemp, Warning, TEXT("LookAtRotation.Pitch : %f"), LookAtRotation.Pitch);

    LookAtRotation.Roll = 90;
    TurretMeshLeft->SetWorldRotation(FMath::RInterpTo(TurretMeshLeft->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 5));
    
    LookAtRotation.Roll = -90,0;
	TurretMeshRight->SetWorldRotation(FMath::RInterpTo(TurretMeshRight->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 5));
	
    LookAtRotation.Roll = 0;
    TurretMeshTop->SetWorldRotation(FMath::RInterpTo(TurretMeshTop->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 5));	

    // LookAtRotation.Pitch = 0;
    TurretMesh->SetWorldRotation(FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, UGameplayStatics::GetWorldDeltaSeconds(this), 5));
}

void ATurretQuadra::CheckFireConditions()
{
    // UE_LOG(LogTemp, Display, TEXT("CheckFireConditions"));
    if(Tank && Tank->bAlive && InFireRange())
    {
        Fire();
    }
}

bool ATurretQuadra::InFireRange()
{
    // UE_LOG(LogTemp, Display, TEXT("InFireRange"));
    float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
    if(Distance <= FireRange)
    {
        return true;
    }
    return false;
}

void ATurretQuadra::Fire()
{
    Super::Fire();
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretQuadra::FireLeft, 0.2, false);
}
	
void ATurretQuadra::FireLeft()
{
    // UE_LOG(LogTemp, Warning, TEXT("FireLeft"));
    FVector SpawnLocation = ProjectileSpawnPointLeft->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPointLeft->GetComponentRotation();
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	Projectile->SetOwner(this);
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretQuadra::FireRight, 0.2, false);
}

void ATurretQuadra::FireRight()
{
    // UE_LOG(LogTemp, Warning, TEXT("FireRight"));
    FVector SpawnLocation = ProjectileSpawnPointRight->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPointRight->GetComponentRotation();
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	Projectile->SetOwner(this);
    
    // UActorComponent* AMovement = Projectile->GetComponentByClass(UProjectileMovementComponent::StaticClass());
    // if(AMovement)
    // {
    //     UProjectileMovementComponent* Movement = Cast<UProjectileMovementComponent>(AMovement);
    //     Movement->MaxSpeed = 100;
    //     Movement->InitialSpeed = 1000;
    // }
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretQuadra::FireTop, 0.2, false);
}

void ATurretQuadra::FireTop()
{
    // UE_LOG(LogTemp, Warning, TEXT("FireTop"));
    FVector SpawnLocation = ProjectileSpawnPointTop->GetComponentLocation();
	FRotator SpawnRotation = ProjectileSpawnPointTop->GetComponentRotation();
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	Projectile->SetOwner(this);
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretQuadra::CheckFireConditions, 1, false);
}

void ATurretQuadra::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* CauserController, AActor* DamageCauser)
{
    // UE_LOG(LogTemp, Warning, TEXT("Damage BOSS"));
    UHealthComponent* Health = FindComponentByClass<UHealthComponent>();
	// Health -= Damage;
	if(Health)
	{
        // FString test = Health->GetHealth().ToString();
        int32 test = Health->GetHealth();
        switch(test)
        {
            case(400):
                // UE_LOG(LogTemp, Warning, TEXT("Damage BOSS 1"));
                // UGameplayStatics::SpawnEmitterAtLocation(this, DamagedParticles, GetActorLocation(), GetActorRotation());
                // static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("ParticleSystem'/Engine/Tutorial/SubEditors/TutorialAssets.TutorialParticleSystem'"));
	            
                // // UParticleSystemComponent* PSC = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MyPSC"));
                // DamagedParticles->SetTemplate(PS.Object);

                break;
            case(300):
                UE_LOG(LogTemp, Warning, TEXT("Damage BOSS 2"));
                break;
            case(200):
                UE_LOG(LogTemp, Warning, TEXT("Damage BOSS 3"));
                break;
            case(100):
                UE_LOG(LogTemp, Warning, TEXT("Damage BOSS 4"));
                break;
            default:
                UE_LOG(LogTemp, Warning, TEXT("Default"));
        }
        
	// 	ToonTankGameMode->ActorDied(DamagedActor);
	} 
}
