// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"



UCLASS()
class ALIEN_SURVIVAL_GAME_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns the WhereToSpawn subobject
	FORCEINLINE UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	// Return random point within BoxComponent
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Pickup to spawn. Anysubclass of Pickup
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer;

	// Min spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayMinimum;

	// Max spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnDelayMaximum;

private:
	// Box component to specify where pickups spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* WhereToSpawn;

	// Handle spawning a new pickup
	void SpawnPickup();

	// Current spawn delay
	float SpawnDelay;
};
