// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class ALIEN_SURVIVAL_GAME_API AHealthPickup : public APickup
{
	GENERATED_BODY()
	

public: 
	// Sets default values for this actor's properties
	AHealthPickup();

	// Override the Collected function - use Implementation because its a blueprint native event
	void Collected_Implementation() override;

	// HealthAmount accessor
	float GetHealthAmount();

protected:
	// Set the amount of health the pickup gives to character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", Meta = (BlueprintProtected = "true"))
	float HealthAmount;
};
