// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Alien_Survival_GameCharacter.generated.h"

UCLASS(Blueprintable)
class AAlien_Survival_GameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAlien_Survival_GameCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
	//Returns CollectionSphere subobject
	FORCEINLINE class USphereComponent* GetCollectionSphere() { return CollectionSphere;  }

	/** Accessor for InitialHealth */
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetInitialHealth();

	/** Accesspr for CharacterHealth */
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetCurrentHealth();

	/** Function to update character health 
	*	@param HealthChange This is the amount to change health by. +ve or -ve.
	*/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float HealthChange);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

	

protected:
	/** Starting health of character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float InitialHealth;

	// Called when we press a key to collect any pickups inside the CollectionSphere
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void HealthColourChange();

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	/** Current health of character */
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float CharacterHealth;

	// Timer to respawn character after set time
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float RespawnTimer;
};

