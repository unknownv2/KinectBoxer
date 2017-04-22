// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "Blueprint/UserWidget.h"
#include "BoxerHudWidget.h"

#include "KinectBoxerGameMode.generated.h"

//enum to store the current state of gameplay
UENUM(BlueprintType)
enum class EBoxerPlayState : uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

/**
 * 
 */
UCLASS()
class DEMO2_API AKinectBoxerGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AKinectBoxerGameMode();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	/** Returns the current playing state */
	UFUNCTION(BlueprintPure, Category = "Boxer")
		EBoxerPlayState GetCurrentState() const;

	/** Sets a new playing state */
	void SetCurrentState(EBoxerPlayState NewState);

	UBoxerHudWidget * GetPlayerWonWidget();
protected:
	/** The Widget class to use for our current playing HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boxer", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetPlayClass;

	/** The Widget class to use for our Player Wins HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boxer", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetWonClass;

	/** The Widget class to use for our Main Menu HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boxer", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetMainMenuClass;

	/** The instance of the HUD */
	UPROPERTY()
		class UUserWidget* CurrentWidget;
private:
	/**Keeps track of the current playing state */
	EBoxerPlayState CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	void Begin();

	/**Handle any function calls that rely upon changing the playing state of our game */
	void HandleNewState(EBoxerPlayState NewState);
};
