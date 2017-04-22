// Fill out your copyright notice in the Description page of Project Settings.

#include "DEMO2.h"
#include "GameManager.h"
#include "SpawnVolume.h"
#include "GameDatabase.h"
#include <math.h>
// Sets default values
AGameManager::AGameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawningEnabled = true;

	// Create the static mesh component
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));

	CurrentRound = 1;
	RootComponent = ObjectMesh;
	BeginGameCounter = 0;
	IsInRound = false;
	RoundTime = 60.0f;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	GetDatabase()->CallFunctionByNameWithArguments(TEXT("welcomeStart"), outputDevice, NULL, true);
}


// Called every frame
float lastTime;
void AGameManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (CurrentRound == 1) {
		SetPlayerScore(hudWidget);
	}
	else if (CurrentRound == 2)
	{
		SetPlayerScore(hudWidget);
	}
	if (IsRoundOver())
		return;

	float timeElapsed = GetWorldTimerManager().GetTimerElapsed(SpawnTimer);

	float fullTime = timeElapsed * 10.0f;
	float rounded = floor(fullTime);
	float finalTime = rounded / 10.f;

	if (lastTime != finalTime)
	{
		lastTime = finalTime;
		//UE_LOG(LogTemp, Error, TEXT("Time is %f"), lastTime);

		float rate = (finalTime / 30.f);
		float lifespan = 3.0f - rate;
		if (finalTime >= 50.0f) {
			lifespan = 2.8f - rate;
		}
		for (auto spawnVolume : SpawnVolumeActors)
		{
			spawnVolume->SetLifeSpan(lifespan);
		}
		//UE_LOG(LogTemp, Error, TEXT("Set spawn lifespans"));
	}
}

void AGameManager::StartRound()
{
	UE_LOG(LogTemp, Error, TEXT("Volume Spawning is %d"), SpawningEnabled);

	bool set = false;

	for (ASpawnVolume* Volume : SpawnVolumeActors)
	{
		if (!set) {
			Volume->SetSpawningActive(true);
			set = true;
		}
	}
	if (set) {
		GetDatabase()->ResetScore();
		IsInRound = true;

		FOutputDeviceNull ar;
		auto db = GetDatabase();
		if (db != nullptr) {
			//db->CallFunctionByNameWithArguments(TEXT("roundStartSoundtrack"), ar, NULL, true);
			if (CurrentRound == 1) {
				SetHudMessage("Player 1 begin!");
				db->CallFunctionByNameWithArguments(TEXT("player1begin"), outputDevice, NULL, true);
			}
			else if (CurrentRound == 2)
			{
				SetHudMessage("Player 2 begin!");
				db->CallFunctionByNameWithArguments(TEXT("player2begin"), outputDevice, NULL, true);
			}
			db->CallFunctionByNameWithArguments(TEXT("roundStartSoundtrack"), outputDevice, NULL, true);
		}
	}
}
bool AGameManager::IsRoundOver()
{	
	//UE_LOG(LogTemp, Warning, TEXT("AGameManager: IsInRound %d"), IsInRound);

	return !IsInRound;	
}
AGameDatabase* AGameManager::GetDatabase()
{
	if (dataBase != nullptr)
		return dataBase;

	TArray<AActor*> FoundDatabases;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameDatabase::StaticClass(), FoundDatabases);
	UE_LOG(LogTemp, Warning, TEXT("AGameManager: Found %d AGameDatabase(s)"), FoundDatabases.Num());
	for (auto database : FoundDatabases)
	{
		if (dataBase == nullptr) {
			dataBase = Cast<AGameDatabase>(database);
		}
		return Cast<AGameDatabase>(database);

	}
	return nullptr;
}
AKinectBoxerGameMode* AGameManager::GetGameMode()
{
	if (gameMode != nullptr)
		return gameMode;

	TArray<AActor*> FoundModes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKinectBoxerGameMode::StaticClass(), FoundModes);
	UE_LOG(LogTemp, Warning, TEXT("AGameManager: Found %d AKinectBoxerGameMode(s)"), FoundModes.Num());
	for (auto mode : FoundModes)
	{
		if (gameMode == nullptr) {
			gameMode = Cast<AKinectBoxerGameMode>(mode);
		}
		return Cast<AKinectBoxerGameMode>(mode);
	}
	return nullptr;
}
void AGameManager::RoundOver()
{
	if(CurrentRound != 2)
		SetHudMessage("Player round has ended.");

	auto db = GetDatabase();
	if (db != nullptr) {
		int score = db->GetScore();
		if (CurrentRound == 1)
		{
			PlayerScores.Push(score);
			Player1Score = score;
			UE_LOG(LogTemp, Error, TEXT("AGameManager: Player 1 score is %d"), Player1Score);
		}
		else if (CurrentRound == 2)
		{
			PlayerScores.Push(score);

			Player2Score = score;
			UE_LOG(LogTemp, Error, TEXT("AGameManager: Player 2 score is %d"), Player2Score);
		}
		db->CallFunctionByNameWithArguments(TEXT("roundComplete"), outputDevice, NULL, true);
		
	}
	GetDatabase()->ResetScore();

	IsInRound = false;
}
void AGameManager::SetHudMessage(FString message)
{
	if(hudWidget != nullptr)
		hudWidget->SetDisplayString(message);
}
void AGameManager::SetPlayerScore(UBoxerHudWidget * userWidget)
{
	if (hudWidget != nullptr && hudWidget != userWidget)
		return;

	//UE_LOG(LogTemp, Warning, TEXT("AGameManager: Setting player score"));

	if (userWidget != nullptr) {
		//UE_LOG(LogTemp, Warning, TEXT("AGameManager: Setting player score"));

		if (hudWidget != userWidget)
		{
			hudWidget = userWidget;
		}
		//hudWidget = userWidget;

		auto db = GetDatabase();
		if (db != nullptr) {
			CurrentPlayerScore = GetDatabase()->GetScore();
			if (CurrentRound == 1) {
				userWidget->SetPlayer1Score(CurrentPlayerScore);
			}
			else if (CurrentRound == 2)
			{
				userWidget->SetPlayer2Score(CurrentPlayerScore);
			}
		}
	}
}
void AGameManager::DisplayWinner()
{
	auto db = GetDatabase();
	FString winnerString;
	if (db != nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AGameManager: Player1Score %d, Player2Score %d"), Player1Score, Player2Score);


		if (Player1Score > Player2Score)
		{
			SetHudMessage("Player 1 wins!");
			winnerString = "Player 1 Wins!";
			db->CallFunctionByNameWithArguments(TEXT("player1wins"), outputDevice, NULL, true);
			UE_LOG(LogTemp, Error, TEXT("AGameManager: Player 1 wins!"));

		}
		else if (Player2Score > Player1Score)
		{
			SetHudMessage("Player 2 wins!");
			winnerString = "Player 2 Wins!";
			db->CallFunctionByNameWithArguments(TEXT("player2wins"), outputDevice, NULL, true);
			UE_LOG(LogTemp, Error, TEXT("AGameManager: Player 2 wins!"));


		}
		else
		{
			SetHudMessage("Player 1 & 2 tied!");
			winnerString = "Players Tied!";
			UE_LOG(LogTemp, Error, TEXT("AGameManager: Players 1 & 2 tied!"));

			//db->CallFunctionByNameWithArguments(TEXT("playerstie"), ar, NULL, true);
		}
	}

	auto game = GetGameMode();
	if (game != nullptr) {
		game->SetCurrentState(EBoxerPlayState::EWon);

		auto widget = game->GetPlayerWonWidget();
		if (widget != nullptr) {
			widget->SetDisplayString(winnerString);
			UE_LOG(LogTemp, Error, TEXT("AGameManager: set player widget"));

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AGameManager: bad widget"));

		}
	}
}
void AGameManager::EndRound()
{
	if (IsRoundOver())
		return;

	UE_LOG(LogTemp, Error, TEXT("AGameManager: End of Round (CurrentRound %d)"), CurrentRound);

	if (CurrentRound == 2)
	{
		// Play game over sound
		//UE_LOG(LogTemp, Warning, TEXT("Game over, displaying score"));
		// PlaySound
		UE_LOG(LogTemp, Error, TEXT("AGameManager: Displaying winner"));
		RoundOver();
		DisplayWinner();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("AGameManager: Found %d AGameDatabase(s)"), FoundDatabases.Num());
		auto db = GetDatabase();
		if (db != nullptr) {
			int score = db->GetScore();

			UE_LOG(LogTemp, Warning, TEXT("Round %d is over. Begin round %d"), CurrentRound, CurrentRound + 1);

			UE_LOG(LogTemp, Warning, TEXT("Current score is %d"), score);
		}
		// disable all spawning
		for (ASpawnVolume* Volume : SpawnVolumeActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Volume Spawning is %d"), SpawningEnabled);
			if (Volume != nullptr){
				Volume->SetSpawningActive(false);
			}
		}
		RoundOver();

		CurrentRound++;
	}
	UE_LOG(LogTemp, Error, TEXT("AGameManager: Round is over (CurrentRound %d)"), CurrentRound);
}

void AGameManager::BeginRound()
{
	if (!IsRoundOver()) // return if in round ended game
		return;

	BeginGameCounter++;
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	//UE_LOG(LogTemp, Warning, TEXT("AGameManager: Found %d SpawnVolume(s)"), FoundActors.Num());

	for (auto Actor : FoundActors)
	{
		//UE_LOG(LogTemp, Warning, TEXT("HIT OBJECT GAMEMANAGER (4)"));

		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor)
		{
			SpawnVolumeActor->SetLifeSpan(2.5f);
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	if (SpawningEnabled)
	{
		StartRound();

		UE_LOG(LogTemp, Warning, TEXT("AGameManager: Set round timer to %f seconds"), RoundTime);

		GetWorldTimerManager().SetTimer(SpawnTimer, this, &AGameManager::EndRound, RoundTime, false);
		//GetWorldTimerManager().SetTimer(HitToBeginGrace, this, &AGameManager::UnlockGameStart, 3.0f, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameManager: Spawning currently disabled, hit again to set timer"), RoundTime);
		EndRound();
	}
	// flip enable disable
	SpawningEnabled = !SpawningEnabled;
}
void AGameManager::NotifyActorBeginOverlap(class AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("AGameManager: ActorBeginOverlap (BeginGameCounter=%d)"), BeginGameCounter);

}
void AGameManager::NotifyActorEndOverlap(class AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	UE_LOG(LogTemp, Warning, TEXT("AGameManager: ActorEndOverlap (BeginGameCounter=%d)"), BeginGameCounter);
	SpawningEnabled = true;
	BeginRound();
}
