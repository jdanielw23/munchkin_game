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

	//HERE TEMPORARILY FOR TESTING - SHOULD BE PRIVATE
	Deck treasureDeck;
	Deck doorDeck;
	Deck discardedDoorCards;
	Deck discardedTreasureCards;

	int getCurrentPlayerTurn() { return playerTurn; }	//Move to private later
	Player* getCurrentPlayer() { return &players[playerTurn]; }
	Deck* getDiscardedTreasureCards() { return &discardedTreasureCards; }
	Deck* getDiscardedDoorCards() { return &discardedDoorCards; }
	Deck* getTreasureDeck() { return &treasureDeck; }
	Deck* getDoorDeck() { return &doorDeck; }
	Card* getCardInPlay() { return cardInPlay; }

	void setGameIsOver(bool gameOver) { gameIsOver = gameOver; }
	void setCardInPlay(Card *aCard) { cardInPlay = aCard; }

	void dealCards();
	int nextPlayersTurn();
	bool isGameOver();
	void addPlayer(Player player);
	Card* bustDownDoor();
	void beginAutoGame();		//Hopefully just for testing
	string playGame();
	int allowBattleMods(int monsterStrength, string &output);		//return modification amount;
	void resetTreasureDeck();
	void resetDoorDeck();

	string getWinningPlayer();
	

private:
	const int CARDS_TO_DEAL = 4;
	vector<Player> players;
	
	int playerTurn;
	int numPlayers;
	Card *cardInPlay;
	bool gameIsOver;

	//PRIVATE FUNCTIONS
	
};

#endif