/**
* EGR 327 Project	CBU
* Tester.cpp		Implementation file for the Test class
* Created 11-13-15	JDW
* This class run tests on all of the functions and writes output to a file
*
* REVISION HISTORY:
* 11-14-15			JDW		Began testing Player class
*/

#include <iostream>
#include <fstream>
#include "Tester.h"
#include "Deck.h"
#include "Card.h"
#include "Player.h"

using namespace std;

Tester::Tester()
{	
	myFile.open("test_output.txt");
	doorDeck = Deck(Card::DeckType::DOOR);
	treasureDeck = Deck(Card::DeckType::TREASURE);
}

Tester::~Tester()
{
	myFile.close();
}

//ONLY PUBLIC METHOD
void Tester::runTests()
{	
	myFile << "*****    DOOR DECK    *****" << endl;
	myFile << printDoorDeckTest() << endl << endl;
	
	myFile << "*****    TREASURE DECK    *****" << endl;
	myFile << printTreasureDeckTest() << endl << endl;

	myFile << "dealCardsTest" << ELLIPSES << (dealCardsFromDeckTest() ? PASS : FAIL) << endl;

	playerTest();
}

//***************     TEST FUNCTIONS     ********************
string Tester::printDoorDeckTest()
{
	return doorDeck.print();
}

string Tester::printTreasureDeckTest()
{
	return treasureDeck.print();
}

bool Tester::dealCardsFromDeckTest()
{
	const int CARDS_TO_DEAL = 5;

	int initialSize = doorDeck.getNumCards();
	Card* lastCardDealt;
	
	for (int i = 0; i < CARDS_TO_DEAL; i++)
	{
		lastCardDealt = doorDeck.dealCard();
	}

	myFile << "lastCardDescription: " << (*lastCardDealt).description << endl;

	int newSize = doorDeck.getNumCards();

	myFile << doorDeck.print() << endl;

	return (newSize == (initialSize - CARDS_TO_DEAL));
}

void Tester::playerTest()
{
	player.receiveCard(treasureDeck.dealCard());

	myFile << "*****    CARDS IN HAND    *****" << endl;
	myFile << player.printCardsInHand() << endl;

	
}