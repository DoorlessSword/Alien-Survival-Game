// Copyright Epic Games, Inc. All Rights Reserved.

#include "Alien_Survival_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Pickup.h"
#include "HealthPickup.h"

AAlien_Survival_GameCharacter::AAlien_Survival_GameCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//set a base health level for the player character
	InitialHealth = 2000.0f;
	CharacterHealth = InitialHealth;
	
	// Create CollectionSphere
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(100.0f);
}

void AAlien_Survival_GameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}

	CollectPickups();
}

// Returns starting health
float AAlien_Survival_GameCharacter::GetInitialHealth()
{
	return InitialHealth;
}

// Returns current health
float AAlien_Survival_GameCharacter::GetCurrentHealth()
{
	return CharacterHealth;
}

// Updates player health when needed
void AAlien_Survival_GameCharacter::UpdateHealth(float HealthChange)
{
	UE_LOG(LogClass, Log, TEXT("current Health is %f"), CharacterHealth)
	CharacterHealth += HealthChange;
	UE_LOG(LogClass, Log, TEXT("New Health is %f"), CharacterHealth)
}

void AAlien_Survival_GameCharacter::CollectPickups()
{
	// Get all overlapping actors and store in array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	// keep track of collected health
	float CollectedHealth = 0.0f;
	
	// for each actor collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected) {
		
		// Cast the actor to APickup
		// Cast used this way returns NULLPTR if cannot cast to base Pickup type
		APickup* const TestPickup = Cast<APickup>(CollectedActors[iCollected]);
		
		// If cast successful && pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive()) {
			// Call pickups Collected function
			TestPickup->Collected();
			// Check to see if pickup is health
			AHealthPickup* const TestHealth = Cast<AHealthPickup>(TestPickup);
			if (TestHealth) {
				// increase collected health
				CollectedHealth += TestHealth->GetHealthAmount();
			}
			// Deactivate pickup
			TestPickup->SetActive(false);
		}
			
	}

	if (CollectedHealth != 0) {
		//Debug string
		UE_LOG(LogClass, Log, TEXT("Collected Health is %f"), CollectedHealth)
		
		UpdateHealth(CollectedHealth);
	}
		
}




