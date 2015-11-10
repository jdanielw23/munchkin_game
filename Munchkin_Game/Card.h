/**
* EGR 327 Project	CBU
* Card.h			Header File for the Card structure
* Created 10-29-15	JDW
* This is the super structure for all of the different types of cards
* REVISION HISTORY:
* 11-01-15			RSR		Added string header to fix compilation errors
*/

#pragma once

#include <string>

using namespace std;

struct Card	{
	enum CardType {DOOR, TREASURE};
	enum ClassType { THIEF, CLERIC, WIZARD, WARRIOR };
	enum RaceType { HALFLING, ELF, DWARF };
	enum SlotType { ONE_HAND, TWO_HANDS, HEADGEAR, FOOTGEAR, ARMOR };
	enum Gender { MALE, FEMALE };
	enum BadStuff {
		LOSE_FOOTGEAR,	//0
		LOSE_HEADGEAR,	//1
		LOSE_LEVEL,		//2
		DEATH,			//3
		LOSE_SMALL_ITEM,	//4
		LOSE_HAND,			//5
		LOSE_PLUS_THREE_ITEM,	//6
		LOSE_ONE_SHOT,		//7
		LOSE_HIGHEST_BONUS,	//8
		LOSE_TWO_LEVELS,	//9
		LOSE_HAND_ITEMS,	//10
		LOSE_HAND_ITEM,		//11
		LOSE_ARMOR,		//12
		LOSE_BIG_ITEM,	//13
		LOSE_TWO_ITEMS	//14
	};

	string title;
	string description;
	CardType cardType;
};

struct MonsterCard : Card {

	int level;
	int numTreasures;
	int numLevels;
	BadStuff badStuff;
	bool altLoseLevel;
	int genderBonus;
	int raceBonus;
	int classBonus;
	ClassType classTypeForBonus;
	RaceType raceTypeForBonus;
	Gender genderTypeForBonus;
};

struct ClassCard : Card {
public:
	ClassType classType;
};

struct RaceCard : Card {
	RaceType raceType;
	string bonus1;
	string bonus2;
};

struct ItemCard : Card {
	ClassCard::ClassType classRestriction;
	RaceCard::RaceType raceRestriction;
	Gender genderRestriction;
	bool fireFlameAttack;
	bool bigItem;
	SlotType slotType;
	int value;
	int bonus;
};

struct OneShotCard : Card {
	bool goUpLevel;
	int value;
	int bonus;
};
