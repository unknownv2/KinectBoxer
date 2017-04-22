// Fill out your copyright notice in the Description page of Project Settings.

#include "DEMO2.h"
#include "SpawnBall.h"
#include "SpawnVolume.h"
#include "GameDatabase.h"
#include "GameManager.h"

void ASpawnBall::NotifyActorBeginOverlap(class AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor == nullptr || WasHit)
		return;
	//FName name = OtherActor->GetClass()->GetFName();
	//if(name != nullptr) {}
	//UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: ActorBeginOverlap with %s"), *name.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: ActorBeginOverlap"));
	//"BP_Avateering_Anim_C"
	if (!OtherActor->IsA(ASpawnVolume::StaticClass()) && !OtherActor->IsA(this->StaticClass()))
	{
		TargetHit();
	}
}
void ASpawnBall::TargetHit()
{
	// play target hit sound
	auto db = GetDatabase();
	if (db != nullptr) {
		db->CallFunctionByNameWithArguments(TEXT("targetHit"), outputDevice, NULL, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: ActorBeginOverlap by player!"));

	WasHit = true;

	this->SetActive(false);

	UE_LOG(LogTemp, Warning, TEXT("ASpawnBall (%llx): Marked for destroy"), this);
}
void ASpawnBall::NotifyActorEndOverlap(class AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
}
AGameDatabase* ASpawnBall::GetDatabase()
{
	if (gameDatabase != nullptr)
		return gameDatabase;

	TArray<AActor*> FoundDatabases;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameDatabase::StaticClass(), FoundDatabases);
	UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: Found %d AGameDatabase(s)"), FoundDatabases.Num());
	for (auto database : FoundDatabases)
	{
		if(gameDatabase == nullptr)
		{
			gameDatabase = Cast<AGameDatabase>(database);
		}
		return Cast<AGameDatabase>(database);
	}
	return nullptr;
}
void ASpawnBall::IncreaseScore()
{
	if (!WasHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: Ball was not hit, no score increase"));
		return;
	}

	// increment score if ball was hit
	GetDatabase()->IncrementScore();
	
}
bool ASpawnBall::IsRoundOver()
{
	// increment score if ball was hit
	TArray<AActor*> FoundManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), FoundManagers);
	UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: Found %d AGameManager(s)"), FoundManagers.Num());
	for (auto gm : FoundManagers)
	{
		return Cast<AGameManager>(gm)->IsRoundOver();
	}
	return false;
}
void ASpawnBall::Destroyed()
{
	PlayerDestroy();
}
void ASpawnBall::PlayerDestroy()
{
	IncreaseScore();

	if (IsRoundOver())
		return;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	int spawnVolumeCount = FoundActors.Num();
	UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: Found %d SpawnVolume(s)"), spawnVolumeCount);

	int n = 0;
	bool set = false;
	int t = -1;
	int rnd = FMath::RandRange(1, spawnVolumeCount);

	for (auto Actor : FoundActors)
	{
		n++;
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			if (SpawnVolumeActor->IsSpawnActive)
			{
				UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: Disabling spawning on volume %d, rnd=%d"), n, rnd);

				t = n;
				SpawnVolumeActor->SetSpawningActive(false);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: Spawning volue is not active, n = %d, rnd=%d"), n, rnd);
				if (!set) {
					while (rnd == t)
					{
						rnd = FMath::RandRange(1, spawnVolumeCount);
					}
					if (n == rnd)
					{
						UE_LOG(LogTemp, Error, TEXT("ASpawnBall: Spawning from volume %d"), n);
						SpawnVolumeActor->SetSpawningActiveWithDelay(true, 0.01f);
						set = true;
					}
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("ASpawnBall(%llx): Exiting destroyed"), this);

	if (!set && spawnVolumeCount > 0)
	{
		while (rnd == t)
		{
			rnd = FMath::RandRange(1, spawnVolumeCount);
		}
		UE_LOG(LogTemp, Error, TEXT("ASpawnBall: Spawning from volume %d (Checked)"), rnd);
		Cast<ASpawnVolume>(FoundActors[rnd - 1])->SetSpawningActiveWithDelay(true, 0.01f);
	}
	UE_LOG(LogTemp, Warning, TEXT("ASpawnBall: Done with destroyed"));
}