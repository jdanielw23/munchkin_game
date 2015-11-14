/**
* EGR 327 Project	CBU
* Tester.cpp		Implementation file for the Test class
* Created 11-13-15	JDW
* This class run tests on all of the functions and writes output to a file
*
* REVISION HISTORY:
* 
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


}

//***************     TEST FUNCTIONS     ********************
string Tester::printDoorDeckTest()
{
	Deck doorDeck(Card::DeckType::DOOR);
	return doorDeck.print();
}

string Tester::printTreasureDeckTest()
{
	Deck treasureDeck(Card::DeckType::TREASURE);
	return treasureDeck.print();
}

bool Tester::dealCardsFromDeckTest()
{
	const int CARDS_TO_DEAL = 5;

	Deck doorDeck(Card::DeckType::DOOR);
	int initialSize = doorDeck.getNumCards();
	Card lastCardDealt;
	
	for (int i = 0; i < CARDS_TO_DEAL; i++)
	{
		lastCardDealt = doorDeck.dealCard();
	}

	myFile << "lastCardDescription: " << lastCardDealt.description << endl;

	int newSize = doorDeck.getNumCards();

	myFile << doorDeck.print() << endl;

	return (newSize == (initialSize - CARDS_TO_DEAL));
}
