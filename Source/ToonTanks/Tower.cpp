// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATower::ATower()
{}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();   
}

void ATower::BeginPlay()
{
	Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    if(Tank)
    {
        GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireConditions, FireRate, true);
    }
    else 
    {
        UE_LOG(LogTemp, Error, TEXT("TOWER.CPP : Error when trying to casting pawn in tank."));
    }
    
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if(Tank && CheckRangeConditions())
    {
        RotateTurret(Tank->GetActorLocation());
    }
}

bool ATower::CheckRangeConditions()
{
    if(InFireRange())
    {
        return true;
    }
    return false;
}

void ATower::CheckFireConditions()
{
    if(Tank->bAlive && InFireRange())
    {
        Fire();
    }
}

bool ATower::InFireRange()
{

    float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
    if(Distance <= FireRange)
    {
        return true;
    }
    return false;
}