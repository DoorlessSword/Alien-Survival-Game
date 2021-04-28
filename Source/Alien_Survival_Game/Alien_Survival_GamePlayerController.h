// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Alien_Survival_GamePlayerController.generated.h"

UCLASS()
class AAlien_Survival_GamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAlien_Survival_GamePlayerController();

protected:
	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void RotateToMouseCursor();

	/** Moves character based on WASD */
	void MoveForward(float Value);
	void MoveRight(float Value);
};


