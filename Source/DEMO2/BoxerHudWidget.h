// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "BoxerHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class DEMO2_API UBoxerHudWidget : public UUserWidget
{
	GENERATED_BODY()
private:
	int Player1Score = 0;
	int Player2Score = 0;
	FString DisplayText = "Waiting to start round...";
public:



	void SetDisplayString(FString displayString);

	UFUNCTION(BlueprintCallable, Category = "BoxerHUD")
		FString GetPlayer1ScoreString() const;

	UFUNCTION(BlueprintCallable, Category = "BoxerHUD")
		FString GetPlayer2ScoreString() const;

	UFUNCTION(BlueprintCallable, Category = "BoxerHUD")
		FString GetDisplayString() const;

	UFUNCTION(BlueprintCallable, Category = "BoxerHUD")
		void SetPlayer1Score(int score);

	UFUNCTION(BlueprintCallable, Category = "BoxerHUD")
		void SetPlayer2Score(int score);
};
