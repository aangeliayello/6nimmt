from django.db import transaction
from sixnimmt.models import Game, Player, PlayerHand, Cards, Board, Row, Move, MoveHistory
import random

def start_new_game(playerIds, gameId=None):
    """
    Starts a new game with given players.
    """
    with transaction.atomic():
        # Create and initialize a new Game instance
        new_game = Game()
        new_game.gameId = generate_game_id() if gameId is None else gameId
        new_game.currentRound = 1
        new_game.totalRounds = 10  # Number of rounds

        # Initialize the game board with empty rows
        game_board = Board()
        game_board.save()
        for _ in range(4):  # Assuming 4 rows as per game rules
            row = Row(capacity=5)
            row.save()
            game_board.rows.add(row)
            
        move_history = MoveHistory()
        move_history.save()

        new_game.boardState = game_board  # Associate the board with the game
        new_game.moveHistory = move_history  # Associate the move history with the game

        new_game.save()  # Save the
        
        # Add players to the game
        for pid in playerIds:
            player, _ = Player.objects.get_or_create(playerId=pid)
            new_game.players.add(player)

        # Initialize and shuffle the deck
        deck = create_and_shuffle_deck()

        # Deal cards to players
        for player in new_game.players.all():
            cards_indices = random.sample(range(len(deck)), 10)  # Deal 10 cards per player
            hand_cards = [deck[i] for i in sorted(cards_indices, reverse=True)]
            player_hand = PlayerHand(player=player)
            player_hand.save()
            for card in hand_cards:
                player_hand.cards.add(card)
                deck.remove(card)

        new_game.save()

    return new_game

def generate_game_id():
    """
    Generates a unique game ID.

    Returns:
        str: A unique game ID.
    """
    return "G" + str(random.randint(1000, 9999))

def create_and_shuffle_deck():
    """
    Creates and shuffles a deck of cards.
    
    Returns:
        list: A shuffled list of saved Cards instances.
    """
    deck = []
    for i in range(1, 105):
        card = Cards(value=i, bull_points=calculate_bull_points(i))
        card.save()  # Save each card instance
        deck.append(card)
    random.shuffle(deck)
    return deck

def calculate_bull_points(card_value):
    """
    Calculates the bull points for a card based on its value.

    Args:
        card_value (int): The value of the card.

    Returns:
        int: The calculated bull points.
    """
    if card_value == 55:
        return 7
    elif card_value % 11 == 0:
        return 5
    elif card_value % 10 == 0:
        return 3
    elif card_value % 5 == 0:
        return 2
    
    return 1

def process_moves(game):
    """
    Processes the current moves and updates the game state.

    Args:
        game (Game): The game instance.
    """
    # Get the current moves
    moves = game.currentMoves.all()
    
    # Sort the moves by card value
    moves = sorted(moves, key=lambda m: m.card.value)
    
    # Get the last card of each row
    row_values = [row.cards.last().value for row in game.boardState.rows.all()]
    
    # Find the row to clean
    row_to_clean = None
    for move in moves:
        if move.card.value < min(row_values):
            row_to_clean = game.boardState.rows.get(cards__value=min(row_values))
            break
    
    # Add the cards to the row
    for move in moves:
        row_to_clean.cards.add(move.card)
    
    # Add the row to the move
    for move in moves:
        move.row_to_clean = row_to_clean
        move.save()
    
    # Remove the cards from the players' hands
    for move in moves:
        player_hand = game.playersHands.get(player=move.player)
        player_hand.cards.remove(move.card)
    
    # Remove the moves from the current moves
    game.currentMoves.clear()
    
    # Check if the row is full
    if row_to_clean.cards.count() == row_to_clean.capacity:
        # Add the row to the move history
        game.moveHistory.add(MoveHistory(moves=moves))
        
        # Remove the row from the board
        game.boardState.rows.remove(row_to_clean)
        
        # Create a new row
        new_row = Row(capacity=5)
        new_row.save()
        game.boardState.rows.add(new_row)
        
        # Increment the round
        game.currentRound += 1
        
        # Check if the game is over
        if game.currentRound > game.totalRounds:
            game.status = Game.Status.FINISHED
            
            game.save()
            return
        
        # Add 3 cards to each player's hand
        for player in game.players.all():
            cards_indices = random.sample(range(len(game.deck)), 3)  # Deal 3 cards per player
            hand_cards = [game.deck[i] for i in sorted(cards_indices, reverse=True)]
            player_hand = game.playersHands.get(player=player)
            for card in hand_cards:
                player_hand.cards.add(card)
                
def make_card_move(game, player_id, card):
    """
    Makes a move by playing a card.

    Args:
        game (Game): The game instance.
        player_id (str): The ID of the player making the move.
        card (int): The card value to play.
    """
    player = Player.objects.get(playerId=player_id)
    player_hand = game.playersHands.get(player=player)
    
    # Validate if player is expected to move
    if game.status != Game.Status.WAITING_FOR_PLAYERS_TO_CHOOSE_CARD and game.currentMoves.filter(player=player).exists():
        raise Exception("Player is not expected to move")
    # Validate if player has the card
    if not player_hand.cards.filter(value=card).exists():
        raise Exception("Player does not have the card")
    
    card_to_play = player_hand.cards.get(value=card)
    player_hand.cards.remove(card_to_play)
    
    # Add move to current moves
    move = Move(card=card_to_play, player=player)
    move.save()
    game.currentMoves.add(move)
    
    # Check if all players have moved
    if game.currentMoves.count() == game.players.count():
        # Check if a player needs to choose a row
        row_values = [row.cards.last().value for row in game.boardState.rows.all()]
        min_row_value = min(row_values)
        for move in game.currentMoves.all():
            if move.card.value < min_row_value:
                game.status = Game.Status.WAITING_FOR_PLAYER_TO_CHOOSE_ROW
                game.save()
                return
    else: # Process moves
        process_moves(game)
    