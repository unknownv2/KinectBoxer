// Fill out your copyright notice in the Description page of Project Settings.

#include "DEMO2.h"
#include "BoxerHudWidget.h"
#include "GameDatabase.h"


FString UBoxerHudWidget::GetPlayer1ScoreString() const
{
	return FString::FromInt(Player1Score);
}
FString UBoxerHudWidget::GetPlayer2ScoreString() const
{
	return FString::FromInt(Player2Score);
}
void UBoxerHudWidget::SetDisplayString(FString displayString)
{
	DisplayText = displayString;
}

FString UBoxerHudWidget::GetDisplayString() const
{
	return DisplayText;
}
void UBoxerHudWidget::SetPlayer1Score(int score)
{
	Player1Score = score;
}
void UBoxerHudWidget::SetPlayer2Score(int score)
{
	Player2Score = score;
}