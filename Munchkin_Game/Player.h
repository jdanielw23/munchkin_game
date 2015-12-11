/**
* EGR 327 Project	CBU
* Player.h			Header File for the Player class
* Created 10-29-15	JDW
* This class contains the main logic for a player in the game.
* REVISION HISTORY:
* 11-01-15			RSR		Renamed variables to prevent conflicts with accessor names.
* 11-01-15			RSR		Changed include to card.h instead of card.cpp
* 11-14-15			JDW		Added some functions 
* 12-07-15		  CSG+RSR	Removed unused destructor
*/
#ifndef PLAYER_H
#define PLAYER_H
#pragma once
#include "Game.h"
#include "Card.h"
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

class Game;

class Player
{
public:
	enum TurnPhase { EQUIPPING, IN_BATTLE, DECIDING, CHARITY, WAITING };

	//CONSTRUCTORS
	Player();

	//ACCESSORS
	string getName() { return name; }
	bool isSuperMunchkin() { return bIsSuperMunchkin; }
	bool isHalfBreed() { return bIsHalfBreed; }
	bool hasBigItem() { return bHasBigItem; }
	int getGear() { return gear; }
	int getLevel() { return level; }
	int getBattleStrength() { return (gear + level); }
	vector<Card*> getCardsInHand() { return cardsInHand; }
	vector<Card*> getEquippedCards() { return equippedCards; }
	TurnPhase getTurnPhase() { return turnPhase; }

	//SETTERS
	void setName(string n) { name = n; }
	void setGender(Card::Gender g) { gender = g; }

	//ACTION FUNCTIONS
	void receiveCard(Card* aCard);
	Card* discardCard(Card* aCard);
	void equipItem(ItemCard* aCard);
	void equipClass(ClassCard* aCard);
	void equipRace(RaceCard* aCard);
	void loseBattle(Game &currentGame, MonsterCard *monster);
	void winBattle(Game &currentGame, MonsterCard *monster);

	//BAD STUFF FUNCTIONS
	bool loseItem(Game &currentGame, Card::SlotType slot);	//return true if item was lost

	//OTHER USEFUL FUNCTIONS
	void setTurnPhase(TurnPhase phase) { turnPhase = phase; }
	int getMonsterStrength(MonsterCard *monster);
	int getModdableAmount();
	bool operator!=(const Player &p);

	//VIRTUAL FUNCTIONS
	virtual void beginTurn(Game &currentGame) {}
	virtual void enterBattlePhase(Game &currentGame, MonsterCard *monster) {}
	virtual void enterDecidingPhase(Game &currentGame) {}
	virtual void enterCharityPhase(Game &currentGame) {}
	virtual void beefMeUp(int &playerStrength) {}
	virtual bool playerWillMod(const int playerLevel, const int modsNeeded) { return false; }

protected:
	const int MAX_LEVEL = 10;
	const int MIN_LEVEL = 1;

	//DATA MEMBERS
	string name;
	int level;
	int gear;
	bool bIsSuperMunchkin;	//a card that allows two classes
	bool bIsHalfBreed;		//a card that allows two races
	bool bHasBigItem;

	vector<Card*> cardsInHand;
	vector<Card*> equippedCards;

	Card::ClassType class1;
	Card::ClassType class2;
	Card::RaceType race1;
	Card::RaceType race2;
	Card::Gender gender;
	map<Card::SlotType, bool> equippedSlots;
	TurnPhase turnPhase;

	//PRIVATE FUNCTIONS	
	void goUpLevel();		//Don't go higher than level 10
	void goDownLevel();		//Don't go lower than level 1
	bool equipItemIsAllowed(const ItemCard &aCard);
	bool equipClassIsAllowed(const ClassCard &aCard);
	bool equipRaceIsAllowed(const RaceCard &aCard);
	void equipCard(Game &currentGame, Card* card);
	map<int, int> printUsableCardsInHand();
	void printOneShotCards();
	void printEquippedCards();

};

#endif