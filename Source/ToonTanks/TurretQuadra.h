// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "TurretQuadra.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATurretQuadra : public ATower
{
	GENERATED_BODY()

public: 

	ATurretQuadra();
	void BeginPlay();
	void Tick(float DeltaTime);
	void RotateTurret(FVector LookAtTarget);
	bool CheckRangeConditions();
	void CheckFireConditions();
	bool InFireRange();
	void Fire();

private:

	void FireLeft();
	void FireRight();
	void FireTop();
	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* CauserController, AActor* DamageCauser);

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMeshLeft;
	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPointLeft;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMeshRight;
	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPointRight;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMeshTop;
	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPointTop;

	FTimerHandle CheckRangeTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Custom")
	class UParticleSystem* DamagedParticles;
	// class UParticleSystemComponent* DamagedParticles;

};
