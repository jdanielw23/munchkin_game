/**
* EGR 327 Project	CBU
* Deck.h			Header File for the Deck class
* Created 10-29-15	JDW
* This class handles shuffling and dealing of a deck of cards
* REVISION HISTORY:
*
*/

#pragma once
#include <vector>
#include "Card.h"

using namespace std;

class Deck
{
private:
	vector<Card> deck;

public:
	Deck();
	Deck(Card::CardType deckType);	//Depending on type, set this from Constants
	~Deck();

	void shuffle();
	Card dealCard();
	void addCard(Card aCard);
};

