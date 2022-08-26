#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" 
#include "GameFramework/DamageType.h" 
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h"
#include "Tank.h"
#include "TurretQuadra.h"
#include "Particles/ParticleSystemComponent.h"
#include "Custom_FieldSystem.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovement->InitialSpeed = 3300;
	ProjectileMovement->MaxSpeed = 3300;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// We add a callback on the Hit event
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HittingComponent, AActor* HittedActor, UPrimitiveComponent* HittedComponent, FVector NormalImpulse, const FHitResult& HitResult)
{
	// UE_LOG(LogTemp, Warning, TEXT("ON HIT %s"), *HittingComponent->GetName());
	// UE_LOG(LogTemp, Warning, TEXT("ON HIT %s"), *HittedComponent->GetName());

	AActor* MyOwner = GetOwner();
	if(MyOwner)
	{
		AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();

		if(HittedActor)
		{
			// UE_LOG(LogTemp, Warning, TEXT("ON HIT %s"), *HittedActor->GetName());
			ACustom_FieldSystem* Test = GetWorld()->SpawnActor<ACustom_FieldSystem>(FieldSystemClass, GetActorLocation(), GetActorRotation());

			UGameplayStatics::ApplyDamage(HittedActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
			if(Cast<ATank>(HittedActor))
			{
				UHealthComponent* Health = Cast<UHealthComponent>(HittedActor->GetDefaultSubobjectByName(TEXT("Health")));
				if(Health) // In case where the actor is the tank, update the health bar HUD
				{
					UpdateTankHealthBar(Health->GetHealth()); 
				}
			}
			if(Cast<ATurretQuadra>(HittedActor))
			{
				UHealthComponent* Health = Cast<UHealthComponent>(HittedActor->GetDefaultSubobjectByName(TEXT("Health")));
				if(Health && Health->GetHealth() == 300)
				{
					UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
				}
			}
			

			if(HitParticles && HitSound && HitCameraShakeClass)
			{
				UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
				UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
			}
		}
	}
	Destroy();

	
}