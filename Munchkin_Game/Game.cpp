/**
* EGR 327 Project	CBU
* Game.cpp			Implementation File for the Game class
* Created 11-30-15	JDW
* This class controls the gameplay.
* REVISION HISTORY:
* 
*/

#include "Game.h"



Game::Game()
{
	const int NUM_SHUFFLES = 7;

	doorDeck = Deck(Card::DeckType::DOOR);
	treasureDeck = Deck(Card::DeckType::TREASURE);
	playerTurn = 0;
	numPlayers = 0;
	gameIsOver = false;
	
	for (int i = 0; i < NUM_SHUFFLES; i++)
	{
		doorDeck.shuffle();
		treasureDeck.shuffle();
	}
	
}


Game::~Game()
{
}

void Game::addPlayer(Player p)
{
	players.push_back(p);
}

void Game::beginAutoGame()
{
	const int NUM_PLAYERS = 4;
	numPlayers = NUM_PLAYERS;

	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		Player comp = Player("Computer" + to_string(i + 1), Player::PlayerType::AI, Card::Gender::MALE);
		addPlayer(comp);
	}

	dealCards();
}

string Game::playGame()
{
	string result = "**********    BEGIN NEW GAME    **********\n";

	while (!gameIsOver)
	{
		result += "\nCurrent Player: " + (*getCurrentPlayer()).getName() +
			"\tLevel: " + to_string((*getCurrentPlayer()).getLevel()) + "\tGear: " +
			to_string((*getCurrentPlayer()).getGear()) + "\n";

		(*getCurrentPlayer()).beginTurn(*this, result);
		nextPlayersTurn();
	}

	result += "\nWINNING PLAYER: " + getWinningPlayer();

	return result;
}

int Game::nextPlayersTurn()
{
	players[playerTurn].setTurnPhase(Player::TurnPhase::WAITING);	//End current player's turn
	playerTurn = (playerTurn + 1) % numPlayers;	//Move to next player
	players[playerTurn].setTurnPhase(Player::TurnPhase::EQUIPPING);	//Begin new player's turn

	return playerTurn;
}

void Game::dealCards()
{
	//Deal Cards to players
	for (int i = 0; i < numPlayers; i++)	//for each player
	{
		for (int j = 0; j < CARDS_TO_DEAL; j++)		//Deal 4 of each card to the players
		{
			if (!doorDeck.isEmpty() && !treasureDeck.isEmpty())
			{
				players[i].receiveCard(doorDeck.dealCard());
				players[i].receiveCard(treasureDeck.dealCard());
			}
		}
		
	}
}

Card* Game::bustDownDoor()
{
	if (doorDeck.isEmpty())
		resetDoorDeck();
	
	cardInPlay = doorDeck.dealCard();

	return cardInPlay;
}

void Game::resetDoorDeck()
{
	if (!discardedDoorCards.isEmpty())
	{		
		for (int i = 0; i < NUM_SHUFFLES; i++)
			discardedDoorCards.shuffle();
		while (!discardedDoorCards.isEmpty())
			doorDeck.addCard(discardedDoorCards.dealCard());
	}
}

void Game::resetTreasureDeck()
{
	if (!discardedTreasureCards.isEmpty())
	{
		for (int i = 0; i < NUM_SHUFFLES; i++)
			discardedTreasureCards.shuffle();
		while (!discardedTreasureCards.isEmpty())
			treasureDeck.addCard(discardedTreasureCards.dealCard());
	}
}

int Game::allowBattleMods(int monsterStrength, string &output)
{
	int newMonsterStrength = monsterStrength;
	int monsterMods = 0;
	bool playerDefeated = false;

	for (unsigned i = 0; i < players.size(); i++)	//Give each player a chance to modify
	{
		if (players[i].getPlayerType() == Player::PlayerType::AI && players[i] != (*getCurrentPlayer()))	//AI actions
		{
			int ableToAdd = players[i].getModdableAmount();
			int neededToStopPlayer = (*getCurrentPlayer()).getBattleStrength() - newMonsterStrength;

			if ((*getCurrentPlayer()).getLevel() > 5 && ableToAdd >= neededToStopPlayer)		//Don't waste modifiers if player is less than level 5
			{
				for (unsigned j = 0; j < players[i].getCardsInHand().size(); j++)
				{
					if ((*players[i].getCardsInHand()[j]).cardType == Card::CardType::ONE_SHOT)
					{
						//Discard each one shot card that the player just used
						OneShotCard *oneShot = dynamic_cast<OneShotCard*>(players[i].getCardsInHand()[j]);

						if (!(*oneShot).goUpLevel)
						{
							newMonsterStrength += (*oneShot).bonus;
							monsterMods += (*oneShot).bonus;
							discardedTreasureCards.addCard(players[i].discardCard(oneShot));
							j -= 1;	//Repair index if one card was removed. This might not work correctly.**********************

							output += "\t\t\t" + players[i].getName() + " Used a One Shot: +" + to_string((*oneShot).bonus) + " for monster.\n";
							output += "\t\t\t\tNew Monster Strength: " + to_string(newMonsterStrength) + "\n";
							if (newMonsterStrength > (*getCurrentPlayer()).getBattleStrength())
							{
								playerDefeated = true;
								break;
							}
						}
					}
				}
			}

		}
		if (playerDefeated)
			break;

	}
	return monsterMods;
}

bool Game::isGameOver()
{
	const int WINNING_LEVEL = 10;

	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i].getLevel() >= WINNING_LEVEL)
			return true;
	}

	return false;
}

string Game::getWinningPlayer()
{
	const int WINNING_LEVEL = 10;

	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i].getLevel() == WINNING_LEVEL)
			return players[i].getName();
	}

	return "NOBODY";
}

