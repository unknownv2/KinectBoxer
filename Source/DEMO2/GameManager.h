// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "BoxerHudWidget.h"
#include "GameDatabase.h"
#include "KinectBoxerGameMode.h"
#include "GameManager.generated.h"

UCLASS()
class DEMO2_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void NotifyActorBeginOverlap(AActor * OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor * OtherActor) override;

	bool started = false;

	/** Return the mesh for the pickup */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ObjectMesh; }
	TArray<class ASpawnVolume*> SpawnVolumeActors;
	bool SpawningEnabled;
	FTimerHandle SpawnTimer;
	FTimerHandle HitToBeginGrace;
	float RoundTime;
	void BeginRound();
	void StartRound();
	void EndRound();
	bool IsRoundOver();
	
	void RoundOver();
	void SetHudMessage(FString message);
	void DisplayWinner();
	UFUNCTION(BlueprintCallable, Category = "GameManagerHUD")
		void SetPlayerScore(UBoxerHudWidget * userWidget);
private:
	/** Static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ObjectMesh;
	AGameDatabase * GetDatabase();
	AKinectBoxerGameMode * GetGameMode();
	TArray<int> PlayerScores;
	int CurrentRound;
	int Player1Score, Player2Score;
	int BeginGameCounter = 0;
	bool IsInRound;
	int CurrentPlayerScore = 0;
	UBoxerHudWidget * hudWidget;
	UBoxerHudWidget * hudWonWidget;

	FOutputDeviceNull outputDevice;
	AGameDatabase * dataBase = nullptr;
	AKinectBoxerGameMode * gameMode = nullptr;
};
