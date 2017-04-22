// Fill out your copyright notice in the Description page of Project Settings.

#include "DEMO2.h"
#include "SpawnObject.h"
#include "SpawnVolume.h"
#include "SUserWidget.h"

// Sets default values
ASpawnObject::ASpawnObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// All pickups start active
	bIsActive = true;

	// Create the static mesh component
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	RootComponent = ObjectMesh;
}

// Called when the game starts or when spawned
void ASpawnObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(!bIsActive)
	{
		Destroy();
	}
}


void ASpawnObject::SetActive(bool NewPickupState)
{
	bIsActive = NewPickupState;
}


bool ASpawnObject::IsActive()
{
	return bIsActive;
}