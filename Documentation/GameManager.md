# AGameManager

Base class for managing the game, such as game modes, HUD, and starting/ending rounds.

## Functions
Name     |   Description
-----------|--------
**EndRound**       | Called when round ends, updates hud and stops all spawning.
**BeginRound**       | Begins randomized spawning for the targets and round timer.
**NotifyActorEndOverlap**       | For starting/ending rounds
**DisplayWinner**       | Determine who the winner is and display the winner's HUD
**SetHudMessage**       | Set the current title for the HUD.
**GetGameMode**       | Determine the current game mode.
**RoundOver**       |  Update the game's HUD after round ends.
**GetDatabase**       | Get the level's DataBase class.
**IsRoundOver**       |  Determine if the player is active in a round or not.
**StartRound**       |  Called when the game starts a new round.
**Tick**       | Update the player's score and set new spawn target time delay based on round elapsed time.
