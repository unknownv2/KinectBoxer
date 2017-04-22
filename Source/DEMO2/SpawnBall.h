// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SpawnObject.h"
#include "GameDatabase.h"

#include "SpawnBall.generated.h"

/**
 * 
 */
UCLASS()
class DEMO2_API ASpawnBall : public ASpawnObject
{
	GENERATED_BODY()
private:

	bool WasHit;
	void IncreaseScore();
	bool IsRoundOver();
	FOutputDeviceNull outputDevice;
	bool secondOverlap = false;
	AGameDatabase * gameDatabase = nullptr;
	AGameDatabase * GetDatabase();
	void TargetHit();
	void PlayerDestroy();
public:

	
	virtual void NotifyActorBeginOverlap(AActor * OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor * OtherActor) override;
	virtual void Destroyed() override;
};
