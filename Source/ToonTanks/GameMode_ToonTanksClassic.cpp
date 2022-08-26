// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode_ToonTanksClassic.h"
#include "Kismet/GameplayStatics.h"
#include "TankPlayerController.h"
#include "Tank.h"
#include "Tower.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h" 
#include "Components/AudioComponent.h" 
#include "GameFramework/GameUserSettings.h" 

// AGameMode_ToonTanksClassic::AGameMode_ToonTanksClassic()
// {
//     int32 musicNumber = FMath::RandRange(1, 10);
    
//     static ConstructorHelpers::FObjectFinder<USoundCue> propellerCue(
//         TEXT("/Content/Assets/Audio/phantom-116107_Cue")
//     );

//     // Store a reference to the Cue asset - we'll need it later.
//     propellerAudioCue = propellerCue.Object;

//     // Create an audio component, the audio component wraps the Cue, 
//     // and allows us to ineract with
//     // it, and its parameters from code.
//     propellerAudioComponent = CreateDefaultSubobject<UAudioComponent>(
//         TEXT("PropellerAudioComp")
//     );
//     // I don't want the sound playing the moment it's created.
//     propellerAudioComponent->bAutoActivate = true;
    
//     // UGameplayStatics::PlaySound2D(GetWorld(), propellerAudioCue, 1, 1, 0);
// }

// void AGameMode_ToonTanksClassic::PostInitializeComponents()
// {
// 	Super::PostInitializeComponents();

// 	if (propellerAudioCue->IsValidLowLevelFast()) {
// 		propellerAudioComponent->SetSound(propellerAudioCue);
// 	}
// }

void AGameMode_ToonTanksClassic::BeginPlay()
{
    Super::BeginPlay();
    UGameUserSettings* gameUserSettings = UGameUserSettings::GetGameUserSettings();
    if(gameUserSettings)
    {
        UE_LOG(LogTemp, Warning, TEXT("GameUserSettings Created"));
        gameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);
    }

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    TankPlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    
    // propellerAudioComponent->Play();

    HandleGameStart();
}


/***
 * Manage start timer
 * Trigger UI
 ***/
void AGameMode_ToonTanksClassic::HandleGameStart()
{
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    TankPlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
    
    TargetTowers = GetTargetTowersCount(); // Count the number of towers to destroy to win the game

    StartGame();

    if (TankPlayerController)
    {
        // Active the controller of the player
        TankPlayerController->SetPlayerEnabledState(false);

        // Time before the game start
        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
            TankPlayerController,
            &ATankPlayerController::SetPlayerEnabledState,
            true
        );
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle,
            PlayerEnableTimerDelegate,
            StartDelay,
            false
        );
    }
}

void AGameMode_ToonTanksClassic::ActorDied(AActor* DeadActor)
{
    UE_LOG(LogTemp, Display, TEXT("actor killed %s"), *DeadActor->GetName());

    if(DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if(TankPlayerController)
        {
            // Tank->DisableInput(Tank->GetTankPlayerController());
            // Tank->GetTankPlayerController()->bShowMouseCursor = false;
            TankPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    } 
    else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        --TargetTowers;
        if(TargetTowers == 0)
        {
            GameOver(true);
        }
    };
    FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AGameMode_ToonTanksClassic::BeginPlay);
}

// Count the number of towers to destroy to win the game
int32 AGameMode_ToonTanksClassic::GetTargetTowersCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}


float AGameMode_ToonTanksClassic::GetStartDelay()
{
    return StartDelay;
}
