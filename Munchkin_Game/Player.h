/**
* EGR 327 Project	CBU
* Player.h			Header File for the Player class
* Created 10-29-15	JDW
* This class contains the main logic for a player in the game.
* REVISION HISTORY:
* 
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include "Card.cpp"

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

	bool isSuperMunchkin;	//a card that allows two classes
	bool isHalfBreed;		//a card that allows two races
	bool hasHireling;		//a card that allows an extra item
	bool inBattle;

	int gear;
	int level;

	//PRIVATE FUNCTIONS
	void equipItem(ItemCard aCard);
	void goUpLevel();		//Don't go higher than level 10
	void goDownLevel();		//Don't go lower than level 1

public:
	enum CardType { ITEM, USABLE_ONCE_ONLY, CLASS, RACE, MONSTER};

	//CONSTRUCTORS
	Player();
	~Player();

	//ACCESSORS
	bool isSuperMunchkin() { return isSuperMunchkin;  }
	bool isHalfBreed() { return isHalfBreed; }
	bool hasHireling() { return hasHireling; }
	bool isInBattle() { return inBattle; }
	int getGear() { return gear; }
	int getLevel() { return level; }
	int getBattleStrength() { return (gear + level); }

	//ACTION FUNCTIONS
	void playCard(Card aCard);	//action will be different based on CardType
	void discardCard(Card aCard);
	void askForHelp();

	
};

