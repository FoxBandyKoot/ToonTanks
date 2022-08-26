// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePowerUp.generated.h"

UCLASS()
class TOONTANKS_API ABasePowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasePowerUp();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTankHealthBar(float UpdatedTankHealth);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Component", BlueprintReadWrite)
	class UStaticMeshComponent* Display1;
	UPROPERTY(EditAnywhere, Category = "Component", BlueprintReadWrite)
	class UStaticMeshComponent* Display2;
	
	UPROPERTY(EditAnywhere, Category = "Component", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollision;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	 	TArray<AActor*> Actors;
	
	UPROPERTY(EditAnywhere, Category = "Caracteristics")
	class USoundBase* OverlapSound;
};
