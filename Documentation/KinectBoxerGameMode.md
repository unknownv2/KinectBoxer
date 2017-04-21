# AKinectBoxerGameMode

Class for managing the current game state such as 

## Functions
Name     |   Description
-----------|--------
**Begin**       | Retrieve handles to all SpawnVolumes on the level and set game mode to Playing.
**GetPlayerWonWidget**       | Get the widget for displaying the winner.
**GetCurrentState**       | Get game mode such as "Playing" or "Won"
**SetCurrentState**       | Set current game mode.
**HandleNewState**       | Handles the start of the game and the end of the game to decide what to show the user.
