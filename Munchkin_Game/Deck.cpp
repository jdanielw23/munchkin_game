/**
* EGR 327 Project	CBU
* Deck.cpp			Implemention File for the Deck class
* Created 10-29-15	JDW
* This class creates each deck of cards from the files
* It also handles shuffling and dealing of a deck of cards
* REVISION HISTORY:
* 11-02-15			JDW		Began file input logic
* 
*/
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Deck.h"

using namespace std;

Deck::Deck()
{
}

Deck::Deck(Card::DeckType deckType)
{
	const string MONSTER_CARDS_FILE = "monster_cards.txt";
	const string CLASS_CARDS_FILE = "class_cards.txt";
	const string RACE_CARDS_FILE = "race_cards.txt";
	const string ITEM_CARDS_FILE = "item_cards.txt";
	const string ONE_SHOT_CARDS_FILE = "one_shot_cards.txt";

	switch (deckType)
	{
		//If required deck type is the Door deck, then add all of the Door cards
	case Card::DeckType::DOOR:
	{
		vector<MonsterCard> monsterCards;
		monsterCards = getMonsterCardsFromFile(MONSTER_CARDS_FILE);
		for (vector<MonsterCard>::iterator it = monsterCards.begin(); it != monsterCards.end(); ++it)
			deck.push(*it);

		vector<ClassCard> classCards;
		classCards = getClassCardsFromFile(CLASS_CARDS_FILE);
		for (vector<ClassCard>::iterator it = classCards.begin(); it != classCards.end(); ++it)
			deck.push(*it);

		vector<RaceCard> raceCards;
		raceCards = getRaceCardsFromFile(RACE_CARDS_FILE);
		for (vector<RaceCard>::iterator it = raceCards.begin(); it != raceCards.end(); ++it)
			deck.push(*it);

		break;
	}

	//If required deck type is the Treasure deck, then add all of the Treasure cards
	case Card::DeckType::TREASURE:
	{
		vector<ItemCard> itemCards;
		itemCards = getItemCardsFromFile(ITEM_CARDS_FILE);
		for (vector<ItemCard>::iterator it = itemCards.begin(); it != itemCards.end(); ++it)
			deck.push(*it);

		vector<OneShotCard> oneShotCards;
		oneShotCards = getOneShotCardsFromFile(ONE_SHOT_CARDS_FILE);
		for (vector<OneShotCard>::iterator it = oneShotCards.begin(); it != oneShotCards.end(); ++it)
			deck.push(*it);

		break;
	}
	default:
		break;
	}
}

Deck::~Deck()
{
}

void Deck::shuffle()
{
	//TODO: Fill in this function
}

Card Deck::dealCard()
{
	Card topCard = deck.top();
	deck.pop();
	return topCard;
}

void Deck::addCard(Card aCard)
{
	//TODO: Fill in this function
}

int Deck::getNumCards()
{
	return deck.size();
}

string Deck::print()
{
	string result = "";
	stack<Card> temp = deck;

	while (!temp.empty())
	{
		Card current = temp.top();
		temp.pop();
		result += current.title + "\n";
	}

	return result;
}

//**************************   PRIVATE MEMBER FUNCTIONS   ********************************

//**************************   MONSTER CARDS   ********************************
vector<MonsterCard> Deck::getMonsterCardsFromFile(string fileName)
{
	ifstream file(fileName);
	string fileLine;
	vector<MonsterCard> cards;

	if (file.is_open())
	{
		while (getline(file, fileLine))
		{
			//Split each line of the text file into each element and assign them to the attributes of the MonsterCard
			vector<string> lineObjects = split(fileLine, ',');
			MonsterCard card;
			card.title = lineObjects.at(0);
			card.description = lineObjects.at(1);
			card.level = stoi(lineObjects.at(2));
			card.numTreasures = stoi(lineObjects.at(3));
			card.numLevels = stoi(lineObjects.at(4));
			card.badStuff = static_cast<Card::BadStuff>(stoi(lineObjects.at(5)));
			card.altLoseLevel = (stoi(lineObjects.at(6)) == 1) ? true : false;
			card.genderBonus = stoi(lineObjects.at(7));
			card.raceBonus = stoi(lineObjects.at(8));
			card.classBonus = stoi(lineObjects.at(9));
			card.genderTypeForBonus = static_cast<Card::Gender>(stoi(lineObjects.at(10)));
			card.raceTypeForBonus = static_cast<Card::RaceType>(stoi(lineObjects.at(11)));
			card.classTypeForBonus = static_cast<Card::ClassType>(stoi(lineObjects.at(12)));
			card.cardType = Card::CardType::MONSTER;

			cards.push_back(card);
		}
		file.close();
	}

	return cards;
}

//**************************   CLASS CARDS   ********************************
vector<ClassCard> Deck::getClassCardsFromFile(string fileName)
{
	const int NUM_CLASS_CARDS = 3;	//There are three of each class card in the deck
	ifstream file(fileName);
	string fileLine;
	vector<ClassCard> cards;

	if (file.is_open())
	{
		while (getline(file, fileLine))
		{
			//Split each line of the text file into each element and assign them to the attributes of the ClassCard
			vector<string> lineObjects = split(fileLine, ',');
			ClassCard card;
			card.title = lineObjects.at(0);
			card.description = lineObjects.at(1);
			card.classType = static_cast<Card::ClassType>(stoi(lineObjects.at(2)));
			card.cardType = Card::CardType::CLASS;

			for (int i = 0; i < NUM_CLASS_CARDS; i++)
				cards.push_back(card);
		}
		file.close();
	}
	return cards;
}

//**************************   RACE CARDS   ********************************
vector<RaceCard> Deck::getRaceCardsFromFile(string fileName)
{
	const int NUM_RACE_CARDS = 3;	//There are three of each race card in the deck
	ifstream file(fileName);
	string fileLine;
	vector<RaceCard> cards;

	if (file.is_open())
	{
		while (getline(file, fileLine))
		{
			//Split each line of the text file into each element and assign them to the attributes of the RaceCard
			vector<string> lineObjects = split(fileLine, ',');
			RaceCard card;
			card.title = lineObjects.at(0);
			card.description = lineObjects.at(1);
			card.raceType = static_cast<Card::RaceType>(stoi(lineObjects.at(2)));
			card.bonus1 = lineObjects.at(3);
			card.bonus2 = lineObjects.at(4);
			card.cardType = Card::CardType::RACE;

			for (int i = 0; i < NUM_RACE_CARDS; i++)
				cards.push_back(card);
		}
		file.close();
	}

	return cards;
}

//**************************   ITEM CARDS   ********************************
vector<ItemCard> Deck::getItemCardsFromFile(string fileName)
{
	ifstream file(fileName);
	string fileLine;
	vector<ItemCard> cards;

	if (file.is_open())
	{
		while (getline(file, fileLine))
		{
			//Split each line of the text file into each element and assign them to the attributes of the ItemCard
			vector<string> lineObjects = split(fileLine, ',');
			ItemCard card;
			card.title = lineObjects.at(0);
			card.description = lineObjects.at(1);
			card.classRestriction = static_cast<Card::ClassType>(stoi(lineObjects.at(2)));
			card.raceRestriction = static_cast<Card::RaceType>(stoi(lineObjects.at(3)));
			card.genderRestriction = static_cast<Card::Gender>(stoi(lineObjects.at(4)));
			card.fireFlameAttack = (stoi(lineObjects.at(5)) == 1) ? true : false;
			card.bigItem = (stoi(lineObjects.at(6)) == 1) ? true : false;
			card.slotType = static_cast<Card::SlotType>(stoi(lineObjects.at(7)));
			card.value = stoi(lineObjects.at(8));
			card.bonus = stoi(lineObjects.at(9));
			card.cardType = Card::CardType::ITEM;

			cards.push_back(card);
		}
		file.close();
	}
	return cards;
}

//**************************   ONE SHOT CARDS   ********************************
vector<OneShotCard> Deck::getOneShotCardsFromFile(string fileName)
{
	ifstream file(fileName);
	string fileLine;
	vector<OneShotCard> cards;

	if (file.is_open())
	{
		while (getline(file, fileLine))
		{
			//Split each line of the text file into each element and assign them to the attributes of the OneShotCard
			vector<string> lineObjects = split(fileLine, ',');
			OneShotCard card;
			card.title = lineObjects.at(0);
			card.description = lineObjects.at(1);
			card.goUpLevel = (stoi(lineObjects.at(2)) == 1) ? true : false;
			card.value = stoi(lineObjects.at(3));
			card.bonus = stoi(lineObjects.at(4));
			card.cardType = Card::CardType::ONE_SHOT;

			cards.push_back(card);
		}
		file.close();
	}
	return cards;
}

//Functions for splitting a string into seperate strings based on a delimiter
vector<string> & Deck::split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> Deck::split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}
