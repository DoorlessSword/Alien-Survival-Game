// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alien_Survival_GamePlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Alien_Survival_GameCharacter.h"
#include "Engine/World.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

AAlien_Survival_GamePlayerController::AAlien_Survival_GamePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAlien_Survival_GamePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AAlien_Survival_GamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AAlien_Survival_GamePlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AAlien_Survival_GamePlayerController::OnSetDestinationReleased);

	// wasd controls
	InputComponent->BindAxis("MoveForward", this, &AAlien_Survival_GamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AAlien_Survival_GamePlayerController::MoveRight);


	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AAlien_Survival_GamePlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AAlien_Survival_GamePlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AAlien_Survival_GamePlayerController::OnResetVR);
}

void AAlien_Survival_GamePlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAlien_Survival_GamePlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AAlien_Survival_GameCharacter* MyPawn = Cast<AAlien_Survival_GameCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AAlien_Survival_GamePlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AAlien_Survival_GamePlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AAlien_Survival_GamePlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AAlien_Survival_GamePlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void AAlien_Survival_GamePlayerController::RotateToMouseCursor()
{

	//APawn* const MyPawn = GetPawn();

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
	if (MyPawn && abs(x) > 0.0f) {
		FVector up = GetPawn()->GetActorLocation();
		up.X += x;
		MyPawn->AddMovementInput(Direction, x);
	}
}

void AAlien_Survival_GamePlayerController::MoveRight(float y)
{
	APawn* const MyPawn = GetPawn();
	FVector Direction(0, 1, 0);
	if (MyPawn && abs(y) > 0.0f) {
		FVector up = GetPawn()->GetActorLocation();
		up.Y += y;
		MyPawn->AddMovementInput(Direction, y);
	}
}

bool AAlien_Survival_GamePlayerController::GetMousePositionOnAimingPlane(FVector& IntersectVector) const
{

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	bool bHit = false;
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
		FVector2D MousePosition;
		if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
		{
			bHit = GetPlanePositionAtScreenPosition(MousePosition, IntersectVector);
		}
	}

	if (!bHit)	//If there was no hit we reset the results. This is redundant but helps Blueprint users
	{
		IntersectVector = FVector::ZeroVector;
	}

	return bHit;
}

bool AAlien_Survival_GamePlayerController::GetPlanePositionAtScreenPosition(const FVector2D ScreenPosition, FVector& IntersectVector) const
{
	// Early out if we clicked on a HUD hitbox
	// Uncomment when a hud has been made
	/*if (GetHUD() != NULL && GetHUD()->GetHitBoxAtCoordinates(ScreenPosition, true))
	{
		return false;
	}*/

	FVector WorldOrigin;
	FVector WorldDirection;
	if (UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection) == true)
	{
		IntersectVector = FMath::LinePlaneIntersection(WorldOrigin, WorldOrigin + WorldDirection * HitResultTraceDistance, GetPawn()->GetActorLocation(), FVector::UpVector);
		return true;
	}

	return false;
}
