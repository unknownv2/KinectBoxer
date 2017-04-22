// Fill out your copyright notice in the Description page of Project Settings.

#include "DEMO2.h"
#include "GameDatabase.h"


// Sets default values
AGameDatabase::AGameDatabase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Score = 0;
}

// Called when the game starts or when spawned
void AGameDatabase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGameDatabase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

