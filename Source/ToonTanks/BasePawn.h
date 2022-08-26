// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();
	void HandleDestruction();

protected:
	
	void RotateTurret(FVector LookAtTarget);
	void Fire();

	UPROPERTY(Category = "Caracteristics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Speed = 1000;
	
	UPROPERTY(Category = "Caracteristics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TurnRate = 100;
	
	UPROPERTY(Category = "Caracteristics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RollRate = 100;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(Category = "Caracteristics", EditDefaultsOnly)
	TSubclassOf<class AProjectile> ProjectileClass; // TSubclassOf<> is used to permit to get the children of a class, here the blueprint of projectile c++ class

private:
	
	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Caracteristics")
	class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Caracteristics")
	class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Caracteristics")
	TSubclassOf<class UCameraShakeBase> DeathCameraShakeClass;
};
