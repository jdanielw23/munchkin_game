/**
* EGR 327 Project	CBU
* Tester.h			Header File for the Test class
* Created 11-13-15	JDW
* This class run tests on all of the functions and writes output to a file
*
* REVISION HISTORY:
* 11-14-15			JDW Added Member variables for easier testing
*/

#include <iostream>
#include <fstream>
#include <string>
#include "Deck.h"
#include "Player.h"
#include "Card.h"
#pragma once

using namespace std;

class Tester
{
private:
	//PRIVATE VARIABLES
	const string PASS = "Pass";
	const string FAIL = "FAILED";
	const string ELLIPSES = "....................";

	ofstream myFile;
	Deck doorDeck;
	Deck treasureDeck;
	Player player;

	//PRIVATE FUNCTIONS
	void printDoorDeckTest();
	void printTreasureDeckTest();
	bool dealCardsFromDeckTest();
	void playerEquippingTest();
	void shuffleTest();
	void gameTest();

public:
	Tester();
	~Tester();

	void runTests();
};

