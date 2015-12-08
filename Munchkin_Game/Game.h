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
#include <string>
#include "Player.h"
#include "Deck.h"
#include "Card.h"

using namespace std;

class Player;

class Game
{
public:
	const int NUM_SHUFFLES = 7;

	Game();
	~Game();

	//ACCESSORS
	int getCurrentPlayerTurn() { return playerTurn; }	//Move to private later
	Player* getCurrentPlayer() { return &players[playerTurn]; }
	Deck* getDiscardedTreasureCards() { return &discardedTreasureCards; }
	Deck* getDiscardedDoorCards() { return &discardedDoorCards; }
	Deck* getTreasureDeck() { return &treasureDeck; }
	Deck* getDoorDeck() { return &doorDeck; }
	Card* getCardInPlay() { return cardInPlay; }

	//SETTERS
	void setGameIsOver(bool gameOver) { gameIsOver = gameOver; }
	void setCardInPlay(Card *aCard) { cardInPlay = aCard; }

	//USEFUL FUNCTIONS
	void addPlayer(Player player);
	void dealCards();
	void beginAutoGame();		//Hopefully just for testing
	string playGame();
	int nextPlayersTurn();
	Card* bustDownDoor();
	int allowBattleMods(int monsterStrength, string &output);		//return modification amount;
	void resetTreasureDeck();
	void resetDoorDeck();

	bool isGameOver();
	string getWinningPlayer();

private:
	const int CARDS_TO_DEAL = 4;
	vector<Player> players;
	Deck treasureDeck;
	Deck doorDeck;
	Deck discardedDoorCards;
	Deck discardedTreasureCards;
	int playerTurn;
	int numPlayers;
	Card *cardInPlay;
	bool gameIsOver;
	
};

#endif