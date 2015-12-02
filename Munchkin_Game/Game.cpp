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

void Game::beginDefaultGame()
{
	numPlayers = 4;
	const int NUM_AI_PLAYERS = 3;
	Player player = Player("Player", Player::PlayerType::HUMAN, Card::Gender::MALE);
	addPlayer(player);

	for (int i = 0; i < NUM_AI_PLAYERS; i++)
	{
		Player comp = Player("Computer" + to_string(i + 1), Player::PlayerType::AI, Card::Gender::MALE);
		addPlayer(comp);
	}

	dealCards();
}

int Game::nextPlayersTurn()
{
	playerTurn++;

	return playerTurn % numPlayers;
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

bool Game::isGameOver()
{
	const int WINNING_LEVEL = 10;

	for (int i = 0; i < numPlayers; i++)
	{
		if (players[i].getLevel() == WINNING_LEVEL)
			return true;
	}

	return false;
}

Card* Game::bustDownDoor()
{
	if (doorDeck.isEmpty())
	{
		const int NUM_SHUFFLES = 7;
		for (int i = 0; i < NUM_SHUFFLES; i++)
		{
			discardedDoorCards.shuffle();
		}
		while (!discardedDoorCards.isEmpty())
			doorDeck.addCard(discardedDoorCards.dealCard());
	}
	
	cardInPlay = doorDeck.dealCard();

	return cardInPlay;
}