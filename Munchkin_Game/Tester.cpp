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

	playerEquippingTest();
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

void Tester::playerEquippingTest()
{
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
}