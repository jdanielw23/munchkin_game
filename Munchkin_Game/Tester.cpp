/**
* EGR 327 Project	CBU
* Tester.cpp		Implementation file for the Test class
* Created 11-13-15	JDW
* This class run tests on all of the functions and writes output to a file
*
* REVISION HISTORY:
*/

#include <iostream>
#include <fstream>
#include <string>
#include "Tester.h"
#include "Deck.h"
#include "Card.h"
#include "Player.h"
#include "Game.h"

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
	
	printDoorDeckTest();
	printTreasureDeckTest();
	gameTest();

	myFile << endl;

	//myFile << "dealCardsTest" << ELLIPSES << (dealCardsFromDeckTest() ? PASS : FAIL) << endl;
	//shuffleTest();
	//playerEquippingTest();


}

//***************     TEST FUNCTIONS     ********************
void Tester::printDoorDeckTest()
{
	myFile << "*****    DOOR DECK    *****" << endl;
	myFile << "*****    SIZE: " + to_string(doorDeck.getNumCards()) + "    *****" << endl;
	myFile << doorDeck.print() << endl;
	myFile << endl;
}

void Tester::printTreasureDeckTest()
{
	myFile << "*****    TREASURE DECK    *****" << endl;
	myFile << "*****    SIZE: " + to_string(treasureDeck.getNumCards()) + "    *****" << endl;
	myFile << treasureDeck.print() << endl;
	myFile << endl;
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

void Tester::shuffleTest()
{
	const int NUM_SHUFFLES = 7;

	for (int i = 0; i < NUM_SHUFFLES; i++)
		doorDeck.shuffle();
	myFile << "*****    DOOR DECK    *****" << endl;
	myFile << "*****    SIZE: " + to_string(doorDeck.getNumCards()) + "    *****" << endl;
	myFile << doorDeck.print() << endl;
	myFile << endl;

	for (int i = 0; i < NUM_SHUFFLES; i++)
		treasureDeck.shuffle();
	myFile << "*****    TREASURE DECK    *****" << endl;
	myFile << "*****    SIZE: " + to_string(treasureDeck.getNumCards()) + "    *****" << endl;
	myFile << treasureDeck.print() << endl;
	myFile << endl;
}

void Tester::gameTest()
{
	Game newGame;
	newGame.beginDefaultGame();
	myFile << "*****    GAME TESTING    *****" << endl;
	myFile << "Player's Turn should be 0: " + to_string(newGame.getCurrentPlayerTurn()) << endl;
	myFile << "Next Player should be 1: " + to_string(newGame.nextPlayersTurn()) << endl;
	myFile << "Next Player should be 2: " + to_string(newGame.nextPlayersTurn()) << endl;
	myFile << "Next Player should be 3: " + to_string(newGame.nextPlayersTurn()) << endl;
	myFile << "Next Player should be 0: " + to_string(newGame.nextPlayersTurn()) << endl;
	myFile << "Current Player should be 0: " + to_string(newGame.getCurrentPlayerTurn()) << endl;
	myFile << endl;

	myFile << "After dealing cards:" << endl;
	myFile << "*****    DOOR DECK    *****" << endl;
	myFile << "*****    SIZE: " + to_string(newGame.doorDeck.getNumCards()) + "    *****" << endl;
	myFile << newGame.doorDeck.print() << endl;

	myFile << "*****    TREASURE DECK    *****" << endl;
	myFile << "*****    SIZE: " + to_string(newGame.treasureDeck.getNumCards()) + "    *****" << endl;
	myFile << newGame.treasureDeck.print() << endl;

	Player* currentPlayer = newGame.getCurrentPlayer();

	myFile << endl << "Player Name: " << (*currentPlayer).getName();

	vector<Card*> playersCards = (*currentPlayer).getCardsInHand();

	myFile << endl << "NUM Cards: " << to_string(playersCards.size()) << endl;

	for (unsigned i = 0; i < playersCards.size(); i++)
	{
		myFile << (*playersCards[i]).title << endl;
	}

}


void Tester::playerEquippingTest()
{
	/*
	string classes[] = {"Thief","Cleric","Wizard","Warrior","None" };
	string races[] = {"Hlflng","Elf","Dwarf","None" };
	string genders[] = {"Male","Female"	};

	player.receiveCard(treasureDeck.dealCard());

	myFile << "*****    CARDS IN HAND    *****" << endl;
	myFile << player.printCardsInHand() << endl;

	myFile << "Class1" << "\t\t" << "Class2" << "\t\t" << "Race1" << "\t\t" << "Race2" << "\t\t" << "Gender" << "\t\t" << "NumAllowed" << endl << endl;

	//Create vector of treasure cards
	vector<ItemCard*> treasureCards;
	while (!treasureDeck.isEmpty())
	{
		treasureCards.push_back(dynamic_cast<ItemCard*>(treasureDeck.dealCard()));
	}

	//All of this craziness creates every possible combination of classes, races, and gender for testing purposes
	bool skipClass = false;
	bool skipRace = false;
	for (int c1 = Card::ClassType::THIEF; c1 <= Card::ClassType::NO_CLASS; c1++)
	{
		Card::ClassType class1 = static_cast<Card::ClassType>(c1);
		for (int c2 = Card::ClassType::THIEF; c2 <= Card::ClassType::NO_CLASS; c2++)
		{
			if (c1 == 4)
				c2 = 4;		//skip rows where 1st class is none, but there is a 2nd class
			else if (c1 == c2)
				skipClass = true;	//can't have 2 of the same class
			else
				skipClass = false;

			Card::ClassType class2 = static_cast<Card::ClassType>(c2);
			for (int r1 = Card::RaceType::HALFLING; r1 <= Card::RaceType::NO_RACE; r1++)
			{
				Card::RaceType race1 = static_cast<Card::RaceType>(r1);
				for (int r2 = Card::RaceType::HALFLING; r2 <= Card::RaceType::NO_RACE; r2++)
				{
					if (r1 == 3)
						r2 = 3;		//skip rows where 1st race is none, but there is a 2nd race
					else if (r1 == r2)
						skipRace = true;	//can't have 2 of the same race
					else
						skipRace = false;

					Card::RaceType race2 = static_cast<Card::RaceType>(r2);
					for (int genderInt = Card::Gender::MALE; genderInt <= Card::Gender::FEMALE; genderInt++)
					{
						Card::Gender gender = static_cast<Card::Gender>(genderInt);
						if (!(skipClass || skipRace))
						{
							myFile << classes[c1] << "\t\t" << classes[c2] << "\t\t" << races[r1] <<
								"\t\t" << races[r2] << "\t\t" << genders[genderInt];

							int numAllowed = 0;
							Player player(class1, class2, race1, race2, gender);
							for (vector<ItemCard*>::iterator it = treasureCards.begin(); it != treasureCards.end(); ++it)
							{
								if (player.equipIsAllowed((*(*it))))
									numAllowed++;
								//myFile << classes[c1] << "\t\t" << classes[c2] << "\t\t" << races[r1] <<
								//	"\t\t" << races[r2] << "\t\t" << genders[genderInt] << "\t\t" << (*(*it)).description << "\t\t" <<( (player.equipIsAllowed((*(*it))))?"true" : "false" )<< endl;
							}
							myFile << "\t\t" << to_string(numAllowed) << endl;
						}
							
					}
				}
			}
		}
	}
	*/
}