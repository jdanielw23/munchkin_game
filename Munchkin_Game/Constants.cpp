/**
* EGR 327 Project	CBU
* Constants.cpp		Implementation File for the Constants class
* Created 10-29-15	JDW
* This is a class to control the constant Door Card Deck and Treasure Card Deck
* Values should be retrieved from either an outside file or a database
* REVISION HISTORY:
*
*/

#include "Constants.h"


Constants::Constants()
{
	//This is just an example of the code required to make one simple card
	UseOnceOnlyCard goldenApples;
	goldenApples.title = "Golden Apples";
	goldenApples.description = "Play during any combat. +3 to either side. Usable once only.";
	goldenApples.value = 500;
	goldenApples.bonus = 3;
	goldenApples.goUpLevel = false;

	const vector<Card> TREASURE_DECK = { goldenApples };
	const vector<Card> DOOR_DECK = {};
}


Constants::~Constants()
{
}
