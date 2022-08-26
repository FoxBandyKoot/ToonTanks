// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h" 
#include "Tank.h"

void ATankPlayerController::SetPlayerEnabledState(bool bPlayerEnable)
{
    if(bPlayerEnable)
    {
        UE_LOG(LogTemp, Warning, TEXT("EnableInput"));
        GetPawn()->EnableInput(this);
    }
    else
    {   
        UE_LOG(LogTemp, Warning, TEXT("DisableInput"));
        GetPawn()->DisableInput(this);
    }
    bShowMouseCursor = bPlayerEnable;
    // Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    
    // int32 SizeX = 0;
    // int32 SizeY = 0;
    // GetViewportSize(SizeX, SizeY);
    // SetMouseLocation(SizeX / 2, SizeY / 2);

}

	// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
    // UE_LOG(LogTemp, Warning, TEXT("TICK CONTROLLER"));
    // UActorComponent* ActorComponent = Tank->GetComponentByClass(USpringArmComponent::StaticClass());
    // if(ActorComponent)
    // {
    //     USpringArmComponent* SpringArmComponent = Cast<USpringArmComponent>(ActorComponent);
    //     if(SpringArmComponent)
    //     {
            // float LocationX = 0;
            // float LocationY = 0;
            
            // int32 SizeX = 0;
            // int32 SizeY = 0;
            // GetViewportSize(SizeX, SizeY);
            // GetMousePosition(LocationX, LocationY);
            // if(LocationX >= SizeX / 1.999999)
            // {
            //     SetMouseLocation(SizeX / 1.999999, LocationY);
            // } 
            // else if(LocationX <= SizeX / 2)
            // {
            //     SetMouseLocation(SizeX / 2, LocationY);
            // }
            // if(LocationY >= SizeY / 1.999999)
            // {
            //     SetMouseLocation(LocationX, SizeY / 2);
            // } 
            // else if(LocationY <= SizeY / 2.000001)
            // {
            //     SetMouseLocation(LocationX,SizeY / 2);
            // }
            

            // FVector MouseLocation(LocationX, LocationY, 0);
            
            // FHitResult HitResult;
		    
            // GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
            // // FVector ToTarget = HitResult.ImpactPoint - SpringArmComponent->GetComponentLocation();
            // FVector ToTarget = SpringArmComponent->GetComponentLocation() - MouseLocation;
	
            // FRotator LookAtRotation = ToTarget.Rotation();
            // LookAtRotation.Pitch = 0;
            // LookAtRotation.Roll = 0;
            // LookAtRotation.Yaw *= -20;
            // UE_LOG(LogTemp, Warning, TEXT("%f"), LookAtRotation.Yaw);

            // SpringArmComponent->SetWorldRotation(LookAtRotation, false, nullptr, ETeleportType::ResetPhysics);

            // SpringArmComponent->SetWorldRotation(
            //     FMath::RInterpTo(
            //         SpringArmComponent->GetComponentRotation(),
            //         LookAtRotation,
            //         UGameplayStatics::GetWorldDeltaSeconds(this),
            //         0
            //     )
	        // );
        // }
    //     Movement->MaxSpeed = 100;
    //     Movement->InitialSpeed = 1000;
    // }
}