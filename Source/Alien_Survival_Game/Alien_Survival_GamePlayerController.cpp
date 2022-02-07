// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alien_Survival_GamePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Alien_Survival_GameCharacter.h"
#include "Engine/World.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

AAlien_Survival_GamePlayerController::AAlien_Survival_GamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAlien_Survival_GamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	RotatdeToMouseCursor();
}

void AAlien_Survival_GamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// wasd controls
	InputComponent->BindAxis("MoveForward", this, &AAlien_Survival_GamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AAlien_Survival_GamePlayerController::MoveRight);
}



void AAlien_Survival_GamePlayerController::RotateToMouseCursor()
{

	// Get current mouse rotation
	FVector mouseLocation, mouseDirection;
	this->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);

	// Get local reference to the controller's character and its rotation
	ACharacter* currentChar = this->GetCharacter();
	FRotator charRotation = currentChar->GetActorRotation();

	// Get the rotation of the mouse direction
	FRotator targetRotation = mouseDirection.Rotation();

	// Since I only want to turn the character relative to the ground, 
	// the Yaw is the only change needed.
	FRotator newRot = FRotator(charRotation.Pitch, targetRotation.Yaw, charRotation.Roll);

	currentChar->SetActorRotation(newRot);


}

void AAlien_Survival_GamePlayerController::MoveForward(float x)
{
	APawn* const MyPawn = GetPawn();
	FVector Direction(1, 0, 0);
	if (MyPawn && FMath::Abs(x) > 0.0f) {
		FVector up = GetPawn()->GetActorLocation();
		up.X += x;
		MyPawn->AddMovementInput(Direction, x);
	}
}

void AAlien_Survival_GamePlayerController::MoveRight(float y)
{
	APawn* const MyPawn = GetPawn();
	FVector Direction(0, 1, 0);
	if (MyPawn && FMath::Abs(y) > 0.0f) {
		FVector up = GetPawn()->GetActorLocation();
		up.Y += y;
		MyPawn->AddMovementInput(Direction, y);
	}
}

void AAlien_Survival_GamePlayerController::Disable()
{
	DisableInput(Cast<APlayerController>(this));
}

void AAlien_Survival_GamePlayerController::Enable()
{
	EnableInput(Cast<APlayerController>(this));
}

