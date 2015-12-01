/**
* EGR 327 Project	CBU
* Game.h			Header File for the Game class
* Created 11-30-15	JDW
* This class controls the gameplay.
* REVISION HISTORY:
*
*/

#pragma once
#include <vector>
#include "Player.h"
#include "Deck.h"
#include "Card.h"

using namespace std;

class Game
{
public:
	Game();
	~Game();

	//HERE TEMPORARILY FOR TESTING - SHOULD BE PRIVATE
	Deck treasureDeck;
	Deck doorDeck;


	int getCurrentPlayerTurn() { return playerTurn % numPlayers; }

	void dealCards();
	int nextPlayersTurn();
	bool isGameOver();
	void addPlayer(Player player);
	void beginDefaultGame();		//Hopefully just for testing

private:
	const int CARDS_TO_DEAL = 4;
	vector<Player> players;
	
	int playerTurn;
	int numPlayers;
	Card cardInPlay;
};

