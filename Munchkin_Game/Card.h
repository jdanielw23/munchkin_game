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
	string title;
	string description;
	CardType cardType;

	Card();
	~Card();
};

struct MonsterCard : Card {
	enum BadStuff {
		LOSE_FOOTGEAR,
		LOSE_HEADGEAR,
		LOSE_LEVEL,
		DEATH,
		LOSE_SMALL_ITEM,
		LOSE_HAND,
		LOSE_PLUS_THREE_ITEM,
		LOSE_UOO,
		LOSE_HIGHEST_BONUS,
		LOSE_TWO_LEVELS,
		LOSE_HAND_ITEMS,
		LOSE_HAND_ITEM,
		LOSE_ARMOR,
		LOSE_BIT_ITEM,
		LOSE_TWO_ITEMS
	};

	int level;
	int numTreasures;
	int numLevels;
	BadStuff badStuff;
	bool altLoseLevel;
	int genderBonus;
	int raceBonus;
	int classBonus;
};

struct ClassCard : Card {
public:
	enum ClassType {THIEF, CLERIC, WIZARD, WARRIOR};

	ClassType classType;
};

struct RaceCard : Card {
	enum RaceType {HALFLING, ELF, DWARF};

	RaceType raceType;
};

struct ItemCard : Card {
	enum SlotType {ONE_HAND, TWO_HANDS, HEADGEAR, FOOTGEAR, ARMOR};
	enum Gender {MALE, FEMALE};

	ClassCard::ClassType classRestriction;
	RaceCard::RaceType raceRestriction;
	Gender genderRestriction;
	bool fireFlameAttack;
	bool bigItem;
	SlotType slotType;
	int value;
	int bonus;
};

struct UseOnceOnlyCard : Card {
	bool goUpLevel;
	int value;
	int bonus;
};
