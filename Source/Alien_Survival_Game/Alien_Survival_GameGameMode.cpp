// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alien_Survival_GameGameMode.h"
#include "Alien_Survival_GamePlayerController.h"
#include "Alien_Survival_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAlien_Survival_GameGameMode::AAlien_Survival_GameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAlien_Survival_GamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}