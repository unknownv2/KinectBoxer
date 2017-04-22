// Fill out your copyright notice in the Description page of Project Settings.

#include "DEMO2.h"
#include "SpawnVolume.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
	Start();
}

void ASpawnVolume::Start()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Box Component to represent the spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;

	//Set the spawn delay range
	SpawnDelayRangeLow = 0.0f;
	SpawnDelayRangeHigh = 0.1f;
	IsSpawnActive = false;
	LifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector SpawnOrigin = WhereToSpawn->Bounds.Origin;
	FVector SpawnExtent = WhereToSpawn->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}
void ASpawnVolume::SetSpawningActiveWithDelay(bool bShouldSpawn, float spawnDelay)
{
	if (bShouldSpawn)
	{
		// Set the timer on Spawn Target
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnTarget, spawnDelay, false);
	}
	else
	{
		// clear the timer on Spawn Pickup
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
	IsSpawnActive = bShouldSpawn;
}
void ASpawnVolume::SetSpawningActive(bool bShouldSpawn)
{
	if (bShouldSpawn)
	{
		// Set the timer on Spawn Target
		SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
		GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnTarget, SpawnDelay, false);
	}
	else
	{
		// clear the timer on Spawn Pickup
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}
	IsSpawnActive = bShouldSpawn;
}

void ASpawnVolume::SetObjectLifeSpan(float lifeSpan)
{
	LifeSpan = lifeSpan;
}

void ASpawnVolume::SpawnTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawning object..."));
	IsSpawnActive = true;
	// If we have set something to spawn:
	if (WhatToSpawn != NULL)
	{
		// Check for a valid World: 
		UWorld* const World = GetWorld();
		if (World)
		{
			// Set the spawn parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			// Get a random location to spawn at
			FVector SpawnLocation = GetRandomPointInVolume();

			// Get a random rotation for the spawned item
			FRotator SpawnRotation;
			SpawnRotation.Yaw = 1.0f;// FMath::FRand() * 360.0f;
			SpawnRotation.Pitch = 1.0f; //FMath::FRand() * 360.0f;
			SpawnRotation.Roll = 1.0f; //FMath::FRand() * 360.0f;

			// spawn the object
			AActor* const SpawnedTarget = World->SpawnActor<AActor>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			//SetLifeSpan()

			SpawnedTarget->SetLifeSpan(LifeSpan);
			UE_LOG(LogTemp, Warning, TEXT("Created object to spawn."));

			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			UE_LOG(LogTemp, Warning, TEXT("Created spawn delay."));

			SpawnDelay = 4.0f;
			//GetWorldTimerManager().ClearTimer(SpawnTimer);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnTarget, SpawnDelay, false);

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error:World is NULL!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error:WhatToSpawn is NULL!"));

	}
}


