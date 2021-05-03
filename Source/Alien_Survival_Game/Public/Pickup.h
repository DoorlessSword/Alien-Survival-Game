// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class ALIEN_SURVIVAL_GAME_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Return mesh for pickup
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

	// Return whether pickup is active
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	// Allows other classes to change if pickup is active
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);

	// Function to call when picked up
	UFUNCTION(BlueprintNativeEvent)
	void Collected();
	virtual void Collected_Implementation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// True when pickup can be used. False when deactivated
	bool bIsActive;

private:	
	// Static mesh to represent pickup ingame
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;


};
