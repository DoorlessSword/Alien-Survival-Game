// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

// Set default values
AHealthPickup::AHealthPickup() {
	GetMesh()->SetSimulatePhysics(true);

	// Base health amount in pickup
	HealthAmount = 150.0f;
}

void AHealthPickup::Collected_Implementation()
{
	// Use base pickup behaviour
	Super::Collected_Implementation();
	//Destroy egg. Destroy implemented here because not all pickups may be destroyed on pickup.
	Destroy();
}


// Returns Heakth in pickup
float AHealthPickup::GetHealthAmount()
{
	return HealthAmount;
}
