// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATower();
	void HandleDestruction();
		
protected:
	
	virtual void BeginPlay() override;		
	void Tick(float DeltaTime);	
	bool CheckRangeConditions();
	void CheckFireConditions();
	bool InFireRange();

	FTimerHandle FireRateTimerHandle;
	class ATank* Tank;
	
	UPROPERTY(Category = "Caracteristics", EditAnywhere)
	float FireRange = 2000;
	UPROPERTY(Category = "Caracteristics", EditAnywhere)
	float FireRate = 1;
};
