// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Alien_Survival_GameGameMode.generated.h"

UCLASS(minimalapi)
class AAlien_Survival_GameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAlien_Survival_GameGameMode();

	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;
};



