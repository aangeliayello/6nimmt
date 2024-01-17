import os
import django

# Configure the environment for your Django project
# Replace 'myproject.settings' with your actual project's settings module
os.environ.setdefault('DJANGO_SETTINGS_MODULE', 'boardgames.settings')
django.setup()


from django.db import models

class Player(models.Model):
    playerId = models.CharField(max_length=200)
    def __str__(self):
        return self.playerId

class Cards(models.Model):
    value = models.IntegerField()
    bull_points = models.IntegerField()
    def __str__(self):
        return f"{self.value} [{self.bull_points}]"
    
class PlayerHand(models.Model):
    player = models.ForeignKey(Player, on_delete=models.CASCADE)
    cards = models.ManyToManyField(Cards)
    def __str__(self):
        return f"player: {self.player.playerId} hand: {self.cards}"
    
class Row(models.Model):
    capacity = models.IntegerField()
    cards = models.ManyToManyField(Cards)
    def __str__(self):
        return f"{self.cards}"
    
class Board(models.Model):
    rows = models.ManyToManyField(Row)
    def __str__(self):
        return f"{self.rows}"
       
class Move(models.Model):
    card = models.ForeignKey(Cards, on_delete=models.CASCADE)
    row_to_clean = models.ImageField()
    player = models.ForeignKey(Player, on_delete=models.CASCADE)
    def __str__(self):
        s = f"{self.player}"
        s += f" card: {self.card}" if self.card else ""
        s += f" row: {self.row_to_clean}" if self.row_to_clean else ""
        return s
    
class MoveHistory(models.Model):
    moves = models.ManyToManyField(Move)
    def __str__(self):
        return f"{self.moves}"
    
class Game(models.Model):
    class Status(models.TextChoices):
        WAITING_FOR_PLAYERS_TO_CHOOSE_CARD = 'WAITING_FOR_PLAYERS_TO_CHOOSE_CARD'
        WAITING_FOR_PLAYER_TO_CHOOSE_ROW = 'WAITING_FOR_PLAYER_TO_CHOOSE_ROW'
        FINISHED = 'FINISHED'
        
    gameId = models.CharField(max_length=200)
    status = models.CharField(max_length=50, choices=Status.choices, default=Status.WAITING_FOR_PLAYERS_TO_CHOOSE_CARD)
    
    currentRound = models.IntegerField()
    totalRounds = models.IntegerField()
    
    players = models.ManyToManyField(Player)
    playersHands = models.ManyToManyField(Cards)
    
    boardState = models.ForeignKey(Board, on_delete=models.CASCADE)
    
    moveHistory = models.ForeignKey(MoveHistory, on_delete=models.CASCADE)
    
    lastUpdated = models.DateTimeField(auto_now=True)
    
    def __str__(self):
        return f"{self.gameId} {self.status} {self.players}"
