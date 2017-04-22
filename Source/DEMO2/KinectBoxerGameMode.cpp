// Fill out your copyright notice in the Description page of Project Settings.

#include "DEMO2.h"
#include "KinectBoxerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"


AKinectBoxerGameMode::AKinectBoxerGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Started GAME MODE: KinectBoxer"));
}

void AKinectBoxerGameMode::BeginPlay()
{
	Super::BeginPlay();
	Begin();
}
void AKinectBoxerGameMode::Begin()
{
	// find all spawn volume Actors
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);
	UE_LOG(LogTemp, Warning, TEXT("GAME MODE: Found %d SpawnVolume(s)"), FoundActors.Num());

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("GAME MODE: Attempting to do HUD work..."));

	SetCurrentState(EBoxerPlayState::EPlaying);
}
void AKinectBoxerGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
UBoxerHudWidget * AKinectBoxerGameMode::GetPlayerWonWidget()
{
	return Cast<UBoxerHudWidget>(CurrentWidget);
}
EBoxerPlayState AKinectBoxerGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AKinectBoxerGameMode::SetCurrentState(EBoxerPlayState NewState)
{
	//set current state
	CurrentState = NewState;
	// handle the new current state
	HandleNewState(CurrentState);
}

void AKinectBoxerGameMode::HandleNewState(EBoxerPlayState NewState)
{
	switch (NewState)
	{
		// If the game is playing
	case EBoxerPlayState::EPlaying:
	{
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->RemoveFromViewport();

			//CurrentWidget->RemoveFromParent();
		}
		if (HUDWidgetPlayClass != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("GAME MODE: Creating Play HUD widget"));
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetPlayClass);
			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
				UE_LOG(LogTemp, Warning, TEXT("GAME MODE: Added widget to viewport"));

			}
		}
		// spawn volumes active
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Volume Spawning set to inactive"));

			Volume->SetSpawningActive(false);
		}
	}
	break;

	// If we've won the game
	case EBoxerPlayState::EWon:
	{
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->RemoveFromViewport();
			//CurrentWidget->RemoveFromParent();
		}
		if (HUDWidgetWonClass != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("GAME MODE: Creating HUD WON widget"));
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetWonClass);
			if (CurrentWidget != nullptr)
			{
				CurrentWidget->AddToViewport();
				UE_LOG(LogTemp, Warning, TEXT("GAME MODE: Added widget to viewport"));
			}
		}
		// spawn volumes inactive
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Volume Spawning set to inactive"));

			Volume->SetSpawningActive(false);
		}

	}
	break;

	// If we've lost the game
	case EBoxerPlayState::EGameOver:
	{
		UE_LOG(LogTemp, Warning, TEXT("Volume Spawning set to inactive"));

		// spawn volumes inactive
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			Volume->SetSpawningActive(false);
		}
	}
	break;

	// Unknown/default state
	default:
	case EBoxerPlayState::EUnknown:
	{
		// do nothing
	}
	break;
	}
}
