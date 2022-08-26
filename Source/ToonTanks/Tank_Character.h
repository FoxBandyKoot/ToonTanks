// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tank_Character.generated.h"

UCLASS()
class TOONTANKS_API ATank_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATank_Character();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestruction();

	bool bAlive = true;

private:

	void MoveForward(float Value);
	void MoveLateral(float Value);
	void Turn(float Value);
	void Roll(float Value);
	void RotateTurret(FVector LookAtTarget);
	void Fire();
	void LookLateral(float Axis);
	void LookUp(float AxisValue); // 


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
	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* MotorParticles;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	
	UPROPERTY(Category = "Caracteristics", EditDefaultsOnly)
	TSubclassOf<class AProjectile> ProjectileClass; // TSubclassOf<> is used to permit to get the children of a class, here the blueprint of projectile c++ class

	UPROPERTY(Category = "Caracteristics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float Speed = 1000;
	
	UPROPERTY(Category = "Caracteristics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float TurnRate = 100;
	
	UPROPERTY(Category = "Caracteristics", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RollRate = 100;

	APlayerController* TankPlayerController;

	UPROPERTY(EditAnywhere);
	float RotationRate = 20;
};
