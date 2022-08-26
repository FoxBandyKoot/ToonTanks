// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePowerUp.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthComponent.h" 

// Sets default values
ABasePowerUp::ABasePowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Display1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Display1"));
	Display1->SetupAttachment(BoxCollision);
	Display2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Display2"));
	Display2->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void ABasePowerUp::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void ABasePowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    GetOverlappingActors(Actors);
    if(Actors.Num() > 0)
    {
        for(AActor* Actor : Actors)
        {     
			UHealthComponent* Health = Cast<UHealthComponent>(Actor->GetDefaultSubobjectByName(TEXT("Health")));
			if(Health && OverlapSound) // Update the health bar HUD
			{
				Health->SetHealth(100);
				UGameplayStatics::PlaySoundAtLocation(this, OverlapSound, GetActorLocation());
				UpdateTankHealthBar(Health->GetHealth()); // Event BP defined in the BP child
				Destroy();
			} else 
			{
				UE_LOG(LogTemp, Warning, TEXT("Error when casting HealComponent or no OverlapSound setted on BasePowerUp"));
			}
		}
	}

	FRotator DeltaRotation(0.f);
	DeltaRotation.Yaw = DeltaTime * 100.0f;
	AddActorLocalRotation(DeltaRotation, true);
}

