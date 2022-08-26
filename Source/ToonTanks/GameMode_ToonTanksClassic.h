// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode_ToonTanksClassic.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API AGameMode_ToonTanksClassic : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void ActorDied(AActor* DeadActor);
	
	// AGameMode_ToonTanksClassic();
	UFUNCTION(BlueprintCallable)
	float GetStartDelay();

protected:

	void BeginPlay();

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:
	
	void HandleGameStart();
	int32 GetTargetTowersCount();
	// void PostInitializeComponents();

	class ATank* Tank;
	class ATankPlayerController* TankPlayerController;

	float StartDelay = 1;
	int32 TargetTowers = 0;
	class USoundCue* propellerAudioCue;
	class UAudioComponent* propellerAudioComponent;

};