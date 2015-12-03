/**
* EGR 327 Project	CBU
* Game.h			Header File for the Game class
* Created 11-30-15	JDW
* This class controls the gameplay.
* REVISION HISTORY:
*
*/
#ifndef GAME_H
#define GAME_H
#pragma once
#include <vector>
#include "Player.h"
#include "Deck.h"
#include "Card.h"

using namespace std;

class Player;

class Game
{
public:
	Game();
	~Game();

	//HERE TEMPORARILY FOR TESTING - SHOULD BE PRIVATE
	Deck treasureDeck;
	Deck doorDeck;
	Deck discardedDoorCards;
	Deck discardedTreasureCards;

	int getCurrentPlayerTurn() { return playerTurn; }	//Move to private later
	Player* getCurrentPlayer() { return &players[playerTurn]; }
	Deck* getDiscardedTreasureCards() { return &discardedTreasureCards; }
	Deck* getDiscardedDoorCards() { return &discardedDoorCards; }

	void dealCards();
	int nextPlayersTurn();
	bool isGameOver();
	void addPlayer(Player player);
	Card* bustDownDoor();
	void beginDefaultGame();		//Hopefully just for testing
	void playGame();
	int allowBattleMods(int monsterStrength);		//return modification amount;

	

private:
	const int CARDS_TO_DEAL = 4;
	vector<Player> players;
	
	int playerTurn;
	int numPlayers;
	Card *cardInPlay;

	//PRIVATE FUNCTIONS
	
};

#endif