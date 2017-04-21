# ASpawnBall

Base class for the BoxerBall which are the targets the player hits.

## Functions
Name     |   Description
-----------|--------
**Begin**       | Retrieve handles to all SpawnVolumes on the level and set game mode to Playing.
**NotifyActorBeginOverlap**       | Handle the ball interacting with another actor.
**TargetHit**       | Called when the ball was hit by a player.
**GetDatabase**       | Get the current DataBase class for the game.
**IncreaseScore**       | Increase the current player's score by 1.
**IsRoundOver**       | Determine whether we are in a round or waiting to start one.
**PlayerDestroy**       | Remove the ball from the level, and spawn a ball in a different location.
