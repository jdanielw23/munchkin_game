/**
* EGR 327 Project	CBU
* Card.h			Header File for the Card structure
* Created 10-29-15	JDW
* This is the super structure for all of the different types of cards
* REVISION HISTORY:
* 11-01-15			RSR		Added string header to fix compilation errors
*/
#ifndef CARD_H
#define CARD_H
#pragma once

#include <string>

using namespace std;

struct Card	{
	enum DeckType {
		DOOR,		//0
		TREASURE	//1
	};
	enum CardType {
		MONSTER,	//0
		RACE,		//1
		CLASS,		//2
		ONE_SHOT,	//3
		ITEM		//4
	};
	enum ClassType { 
		THIEF,		//0
		CLERIC,		//1
		WIZARD,		//2
		WARRIOR,	//3
		NO_CLASS	//4
	};
	enum RaceType { 
		HALFLING,	//0
		ELF,		//1
		DWARF,		//2
		NO_RACE		//3
	};
	enum RestrictionKey {
		NOT_USABLE_BY_CLASS,
		NOT_USABLE_BY_RACE,
		NOT_USABLE_BY_GENDER,
		USABLE_BY_CLASS_ONLY,
		USABLE_BY_RACE_ONLY,
		USABLE_BY_GENDER_ONLY,
		NO_RESTRICTION
	};
	enum SlotType {
		ONE_HAND,	//0
		TWO_HANDS,	//1
		HEADGEAR,	//2
		FOOTGEAR,	//3
		ARMOR,		//4
		NONE		//5
	};
	enum Gender { 
		MALE,	//0
		FEMALE	//1
	};
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
	const string cardTypes[5] = {"MONSTER","RACE","CLASS","ONE-SHOT","ITEM"};


	string title;
	string description;
	CardType cardType;

	virtual string print() { return title + "\n";  }
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

	string print() { return (title + "   " + cardTypes[cardType] +
		"   Level: " + to_string(level) + "\n"); }
};

struct ClassCard : Card {
public:
	ClassType classType;

	string print() { return (title + "   " + cardTypes[cardType] + "\n"); }
};

struct RaceCard : Card {
	RaceType raceType;
	string bonus1;
	string bonus2;

	string print() { return (title + "   " + cardTypes[cardType] + "\n"); }
};

struct ItemCard : Card {
	RestrictionKey restrictionKey;
	ClassType classRestriction;
	RaceType raceRestriction;
	Gender genderRestriction;
	bool fireFlameAttack;
	bool bigItem;
	SlotType slotType;
	int value;
	int bonus;

	string print() { return (title + "   " + cardTypes[cardType] +
		"   Bonus: +" + to_string(bonus) + "\n"); }
};

struct OneShotCard : Card {
	bool goUpLevel;
	int value;
	int bonus;

	string print() { return (title + "   " + cardTypes[cardType] +
		(goUpLevel ? "   Go Up a Level" : "   Value : $" + to_string(value) + 
			"   Bonus: +" + to_string(bonus) ) +"\n"); }
};

#endif