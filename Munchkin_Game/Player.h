/**
* EGR 327 Project	CBU
* Player.h			Header File for the Player class
* Created 10-29-15	JDW
* This class contains the main logic for a player in the game.
* REVISION HISTORY:
* 11-01-15			RSR		Renamed variables to prevent conflicts with accessor names.
* 11-01-15			RSR		Changed include to card.h instead of card.cpp
* 11-14-15			JDW		Added some functions 
*/

#pragma once
#include <string>
#include <vector>
#include <map>
#include "Card.h"

using namespace std;
class Player
{
public:
	enum PlayerType { AI, HUMAN };
	enum TurnPhase { EQUIPPING, IN_BATTLE, DECIDING, CHARITY, WAITING };

	//CONSTRUCTORS
	Player();
	Player(string n, PlayerType p, Card::Gender g);

	~Player();

	//ACCESSORS
	bool isSuperMunchkin() { return bIsSuperMunchkin; }
	bool isHalfBreed() { return bIsHalfBreed; }
	bool isInBattle() { return bInBattle; }
	bool hasBigItem() { return bHasBigItem; }
	int getGear() { return gear; }
	int getLevel() { return level; }
	int getBattleStrength() { return (gear + level); }
	vector<Card*> getCardsInHand() { return cardsInHand; }
	vector<Card*> getEquippedCards() { return equippedCards; }

	//ACTION FUNCTIONS
	void playCard(Card* aCard);	//action will be different based on CardType

	void receiveCard(Card* aCard);
	void discardCard(Card* aCard);
	void askForHelp();

	void loseBattle();
	bool winBattle();	//return true if player has reached level 10

	void equipClass(ClassCard* aCard);
	void equipRace(RaceCard* aCard);
	void equipItem(ItemCard* aCard);
	void loseItem(ItemCard* aCard);

	void setSuperMunchkin(bool super) { bIsSuperMunchkin = super; }
	void setHalfBreed(bool half) { bIsHalfBreed = half; }

	bool equipIsAllowed(const ItemCard &aCard);

	//TODO: Here for debugging only 
	string printCardsInHand();
	
private:
	const int MAX_LEVEL = 10;
	const int MIN_LEVEL = 1;

	//DATA MEMBERS
	string name;
	PlayerType pType;

	vector<Card*> cardsInHand;
	vector<Card*> equippedCards;

	Card::ClassType class1;
	Card::ClassType class2;
	Card::RaceType race1;
	Card::RaceType race2;
	Card::Gender gender;
	map<Card::SlotType, bool> equippedSlots;
	TurnPhase turnPhase;

	bool bIsSuperMunchkin;	//a card that allows two classes
	bool bIsHalfBreed;		//a card that allows two races
	bool bInBattle;
	bool bHasBigItem;

	int gear;
	int level;

	//PRIVATE FUNCTIONS	
	void goUpLevel();		//Don't go higher than level 10
	void goDownLevel();		//Don't go lower than level 1



};

