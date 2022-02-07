// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "AIVision.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Initail Values
	SightLine = 100.0f;

}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Returns Enemy Sight Line
float AEnemyAI::GetSightLine()
{
	return SightLine;
}
