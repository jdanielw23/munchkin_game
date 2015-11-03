/**
* EGR 327 Project	CBU
* Deck.cpp			Implemention File for the Deck class
* Created 10-29-15	JDW
* This class handles shuffling and dealing of a deck of cards
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

Deck::Deck(Card::CardType deckType)
{
	const string MONSTER_CARDS_FILE = "monster_cards.txt";
	const string CLASS_CARDS_FILE = "class_cards.txt";
	const string RACE_CARDS_FILE = "race_cards.txt";
	const string ITEM_CARDS_FILE = "item_cards.txt";
	const string ONE_SHOT_CARDS_FILE = "one_shot_cards.txt";

	switch (deckType)
	{
	case Card::CardType::DOOR:
	{
		//If required deck type is the Door deck, then add all of the Door cards

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
	case Card::CardType::TREASURE:
	{
		//If required deck type is the Treasure deck, then add all of the Treasure cards

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
			card.badStuff = static_cast<MonsterCard::BadStuff>(stoi(lineObjects.at(4)));
			card.altLoseLevel = stoi(lineObjects.at(5));
			card.genderBonus = stoi(lineObjects.at(6));
			card.raceBonus = stoi(lineObjects.at(7));
			card.classBonus = stoi(lineObjects.at(8));

			cards.push_back(card);
		}
		file.close();
	}

	return cards;
}

vector<ClassCard> Deck::getClassCardsFromFile(string fileName)
{
	vector<ClassCard> cards;
	return cards;
}

vector<RaceCard> Deck::getRaceCardsFromFile(string fileName)
{
	vector<RaceCard> cards;
	return cards;
}

vector<ItemCard> Deck::getItemCardsFromFile(string fileName)
{
	vector<ItemCard> cards;
	return cards;
}

vector<OneShotCard> Deck::getOneShotCardsFromFile(string fileName)
{
	vector<OneShotCard> cards;
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
