# myapp/views.py
from django.http import JsonResponse, HttpResponseBadRequest
from django.views.decorators.http import require_http_methods
from django.views.decorators.csrf import csrf_exempt
import json

from .game_service import start_new_game
from .models import Game

def index(request):
    return JsonResponse({'message': 'Welcome to Six Nimmt!'})

@csrf_exempt
@require_http_methods(["POST"])
def start_game(request):
    try:
        data = json.loads(request.body)
        game_id = data.get('gameId')
        player_ids = data.get('playerIds')

        if not game_id or not player_ids:
            return HttpResponseBadRequest("Missing gameId or playerIds")

        # Assuming start_new_game now also takes the game_id as an argument
        game = start_new_game(playerIds=player_ids, gameId=game_id)
        return JsonResponse({'message': 'Game started', 'game_id': game.gameId})

    except json.JSONDecodeError:
        return HttpResponseBadRequest("Invalid JSON")
    
@csrf_exempt
@require_http_methods(["POST"])
def make_move(request):
    try:
        data = json.loads(request.body)
        gameId = data.get('gameId')
        playerIds = data.get('playerId')
        card = data.get('card')
        row = data.get('row')

        game = Game.objects.get(gameId=gameId)
        
        # validate if player is in game
        if not playerIds in game.players:
            return HttpResponseBadRequest("Player not in game")
        
        # validate if card xor row is given
        if card is None and row is None:
            return HttpResponseBadRequest("Missing card or row")
        if card is not None and row is not None:
            return HttpResponseBadRequest("Card and row given, expected only one")

        if card is not None:
            make_card_move(game, playerIds, card)
        else:
            make_row_move(game, playerIds, row)   

        return JsonResponse({'message': 'Game started', 'game_id': game.gameId})

    except json.JSONDecodeError:
        return HttpResponseBadRequest("Invalid JSON")
    