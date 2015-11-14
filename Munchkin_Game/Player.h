/**
* EGR 327 Project	CBU
* Player.h			Header File for the Player class
* Created 10-29-15	JDW
* This class contains the main logic for a player in the game.
* REVISION HISTORY:
* 11-01-15			RSR		Renamed variables to prevent conflicts with accessor names.
* 11-01-15			RSR		Changed include to card.h instead of card.cpp
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include "Card.h"

using namespace std;

class Player
{
private:
	//DATA MEMBERS
	string name;

	vector<Card> cardsInHand;
	vector<ItemCard> equippedCards;

	vector<ClassCard::ClassType> classes;
	vector<RaceCard::RaceType> races;
	map<ItemCard::SlotType, bool> equippedSlots;

	bool bIsSuperMunchkin;	//a card that allows two classes
	bool bIsHalfBreed;		//a card that allows two races
	bool bHasHireling;		//a card that allows an extra item
	bool bInBattle;

	int gear;
	int level;

	//PRIVATE FUNCTIONS
	void equipItem(ItemCard aCard);		//Probably going to be the most complicated!
	void loseItem(ItemCard aCard);
	void goUpLevel();		//Don't go higher than level 10
	void goDownLevel();		//Don't go lower than level 1

public:
	
	//CONSTRUCTORS
	Player();
	~Player();

	//ACCESSORS
	bool isSuperMunchkin() { return bIsSuperMunchkin;  }
	bool isHalfBreed() { return bIsHalfBreed; }
	bool hasHireling() { return bHasHireling; }
	bool isInBattle() { return bInBattle; }
	int getGear() { return gear; }
	int getLevel() { return level; }
	int getBattleStrength() { return (gear + level); }

	//ACTION FUNCTIONS
	void playCard(Card aCard);	//action will be different based on CardType
	void discardCard(Card aCard);
	void askForHelp();

};

