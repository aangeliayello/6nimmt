# Generated by Django 5.0 on 2024-01-17 16:42

import django.db.models.deletion
from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Board',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
            ],
        ),
        migrations.CreateModel(
            name='Cards',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('value', models.IntegerField()),
                ('bull_points', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Player',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('playerId', models.CharField(max_length=200)),
            ],
        ),
        migrations.CreateModel(
            name='Move',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('row_to_clean', models.ImageField(upload_to='')),
                ('card', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='sixnimmt.cards')),
                ('player', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='sixnimmt.player')),
            ],
        ),
        migrations.CreateModel(
            name='MoveHistory',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('moves', models.ManyToManyField(to='sixnimmt.move')),
            ],
        ),
        migrations.CreateModel(
            name='Game',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('gameId', models.CharField(max_length=200)),
                ('status', models.CharField(choices=[('WAITING_FOR_PLAYERS_TO_CHOOSE_CARD', 'Waiting For Players To Choose Card'), ('WAITING_FOR_PLAYER_TO_CHOOSE_ROW', 'Waiting For Player To Choose Row'), ('FINISHED', 'Finished')], default='WAITING_FOR_PLAYERS_TO_CHOOSE_CARD', max_length=50)),
                ('currentRound', models.IntegerField()),
                ('totalRounds', models.IntegerField()),
                ('lastUpdated', models.DateTimeField(auto_now=True)),
                ('boardState', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='sixnimmt.board')),
                ('playersHands', models.ManyToManyField(to='sixnimmt.cards')),
                ('moveHistory', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='sixnimmt.movehistory')),
                ('players', models.ManyToManyField(to='sixnimmt.player')),
            ],
        ),
        migrations.CreateModel(
            name='PlayerHand',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('cards', models.ManyToManyField(to='sixnimmt.cards')),
                ('player', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='sixnimmt.player')),
            ],
        ),
        migrations.CreateModel(
            name='Row',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('capacity', models.IntegerField()),
                ('cards', models.ManyToManyField(to='sixnimmt.cards')),
            ],
        ),
        migrations.AddField(
            model_name='board',
            name='rows',
            field=models.ManyToManyField(to='sixnimmt.row'),
        ),
    ]
