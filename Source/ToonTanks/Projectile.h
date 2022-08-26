// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USoundBase;

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTankHealthBar(float UpdatedTankHealth);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HittingComponent, AActor* HittedActor, UPrimitiveComponent* HittedComponent, FVector NormalImpulse, const FHitResult& HitResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	class UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystemComponent* TrailParticles;

	
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ACustom_FieldSystem> FieldSystemClass;
};
