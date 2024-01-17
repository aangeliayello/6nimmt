
# 6 Nimmt Game API

## Table of Contents
- [6 Nimmt Game API](#6-nimmt-game-api)
  - [Table of Contents](#table-of-contents)
  - [Endpoints](#endpoints)
    - [Get Game State](#get-game-state)
    - [Make Move](#make-move)
    - [Start Game](#start-game)
  - [Error Handling](#error-handling)

## Endpoints

### Get Game State

- **URL**: `/get_game_state`
- **Method**: `GET`
- **Parameters**: 
  - `gameId`: String, the unique Identifier for the game session.
  - `playerId` [Optional]: String, the Id of the requesting player.
- **Description**: Retrieves the current public state of the game, which includes all information that is not specific to any single player. If playerId is provIded then it also returns the hand of the player
- **Request**: `/get_game_state?gameId=123&playerId=Slovak1`
- **Response**: A JSON object representing the current public game state, including player information, board state, moves history, and the list of players who have made a move in the current round. And, if playerId provIded the respective hand.
  ```json
  {
    "gameId": "String",
    "gameStatus": "String",
    "players": [
      {"playerId": "String", "score": Integer, "hasMoved": Boolean}
    ],
    "boardState": [
      {"row": Integer, "cards": [Integer]}
    ],
    "moveHistory": [
      {"round": Integer, "moves": [{"playerId": "String", "playedCard": Integer},
      {"playerId": "String", "playedCard": Integer, "rowTaken": Integer} ]}
    ],
    "currentRound": Integer,
    "totalRounds": Integer,
    "lastUpdated": "DateTime",
    "hand": [Integer]
  }
  ```


### Make Move

- **URL**: `/make_move`
- **Method**: `POST`
- **Parameters**: None
- **Body**: JSON object containing:
  - `gameId`: String, the unique Identifier for the game session.
  - `playerId`: Integer, the Id of the player making the move.
  - `card` [Optional]: Integer, the card number that the player wishes to play.
  - `row` [Optional]: Integer, the row that wants to be cleaned.
- **Description**: Allows a player to make a move by playing a card. The server will process the move according to the game rules and update the game state. If the player place the lowest card, and the card is lower than all the ankor cards in each row, then, the player is required to make a sub-move to choose the row. 

- **Request:**:
  - Regular move:
  ```json
    {
        "gameId": "game123",
        "playerId": 1,
        "card": 34
    }
  ```
  - Sub-move to clear row.
  ```json
    {
        "gameId": "game123",
        "playerId": 1,
        "row": 1
    }
  ```
- **Response Example**:
  - Success:
  ```json
  {
      "status": "success",
      "message": ":)",
  }
  ```

### Start Game

- **URL**: `/start_game`
- **Method**: `POST`
- **Body**: JSON object containing (optional `gameId` and required list of player Ids):
  - `gameId` (optional): String, a unique Identifier for the game session. If not provIded, the server will generate one.
  - `playerIds`: Array of strings, representing the Ids of the players participating in the game.

- **Description**: Initializes a new game session. If a `gameId` is not provIded, the server generates one. The endpoint sets up the game with the specified players.

- **Request Example**:
  ```json
  {
      "gameId": "game123",
      "players": ["Slovak1", "Slovak2", "Alo"]
  }
  ```

## Error Handling
- **400 Bad Request:** The request is invalid or missing required fields.
- **404 Not Found**: The specified game or player could not be found.
- **409 Conflict**: The move could not be processed due to a conflict with the game state (e.g., playing a card not in the player's hand).
- **500 Internal** Server Error: An unexpected server error occurred. Please try again later.
