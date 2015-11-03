/**
* EGR 327 Project	CBU
* tester.cpp		This is the test driver for testing bits of code
* Created 11-02-15	JDW
* 
* REVISION HISTORY:
* 
*
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "Card.h"

using namespace std;

vector<MonsterCard> getMonsterCardsFromFile(string fileName);
vector<string> & split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);

int main()
{
	/*
	const string MONSTER_CARDS_FILE = "monster_cards.txt";
	vector<MonsterCard> cards = getMonsterCardsFromFile(MONSTER_CARDS_FILE);

	for (vector<MonsterCard>::iterator it = cards.begin(); it != cards.end(); ++it)
	{
		MonsterCard card = *it;
		cout << "Title: " << card.title << endl;
		cout << "Descrip: " << card.description << endl;
	}
	*/

	cout << "Hello World";
	return 0;
}

vector<MonsterCard> getMonsterCardsFromFile(string fileName)
{
	ifstream file(fileName);
	string fileLine;
	vector<MonsterCard> cards;

	if (file.is_open())
	{
		while (getline(file, fileLine))
		{
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
	else
		cout << "ERROR";

	return cards;
}

vector<string> & split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

vector<string> split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}