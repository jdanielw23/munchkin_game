/**
* EGR 327 Project	CBU
* Deck.h			Header File for the Deck class
* Created 10-29-15	JDW
* This class handles shuffling and dealing of a deck of cards
* REVISION HISTORY:
*
*/

#pragma once
#include <stack>
#include <vector>
#include "Card.h"

using namespace std;

class Deck
{
private:
	stack<Card> deck;
	
	vector<MonsterCard> getMonsterCardsFromFile(string fileName);
	vector<ClassCard> getClassCardsFromFile(string fileName);
	vector<RaceCard> getRaceCardsFromFile(string fileName);
	vector<ItemCard> getItemCardsFromFile(string fileName);
	vector<OneShotCard> getOneShotCardsFromFile(string fileName);
	vector<string> &split(const string &s, char delim, vector<string> &elems);
	vector<string> split(const string &s, char delim);

public:
	Deck();
	Deck(Card::CardType deckType);	//Will be either a Door Deck or a Treasure Deck
	~Deck();

	void shuffle();
	Card dealCard();
	void addCard(Card aCard);
};

