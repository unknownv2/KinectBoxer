// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameDatabase.generated.h"

UCLASS()
class DEMO2_API AGameDatabase : public AActor
{
	GENERATED_BODY()

private:

	int Score;
	FOutputDeviceNull outputDevice;

public:	
	// Sets default values for this actor's properties
	AGameDatabase();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "GameDatabase")

	int GetScore() const
	{
		return Score;
	}
	void ResetScore()
	{
		Score = 0;
	}
	void SetScore(int score)
	{
		Score = score;
	}

	void IncrementScore(int incrementToScore)
	{
		Score += incrementToScore;
		UE_LOG(LogTemp, Warning, TEXT("AGameDatabase: incremented score, current score is %d"), Score);

	}
	void IncrementScore()
	{
		Score++;
		UE_LOG(LogTemp, Warning, TEXT("AGameDatabase: incremented score, current score is %d"), Score);
	}
};
