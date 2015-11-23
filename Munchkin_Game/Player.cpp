/**
* EGR 327 Project	CBU
* Player.cpp		Implementation File for the Player class
* Created 10-29-15	JDW
* This class contains the main logic for a player in the game.
* REVISION HISTORY:
*/

#include "Player.h"

Player::Player()
{
	level = MIN_LEVEL;
	gear = level;
	class1 = Card::ClassType::NO_CLASS;
	class2 = Card::ClassType::NO_CLASS;
	race1 = Card::RaceType::NO_RACE;
	race2 = Card::RaceType::NO_RACE;
	bIsSuperMunchkin = false;
	bIsHalfBreed = false;

	//Initialize Slots
	equippedSlots[Card::SlotType::ONE_HAND] = false;
	equippedSlots[Card::SlotType::TWO_HANDS] = false;
	equippedSlots[Card::SlotType::HEADGEAR] = false;
	equippedSlots[Card::SlotType::FOOTGEAR] = false;
	equippedSlots[Card::SlotType::ARMOR] = false;
}

Player::Player(Card::ClassType c1, Card::ClassType c2,
	Card::RaceType r1, Card::RaceType r2, Card::Gender g) : Player()
{
	class1 = c1;
	class2 = c2;
	race1 = r1;
	race2 = r2;
	gender = g;
}

Player::~Player()
{
}

//***************     PUBLIC FUNCTIONS     ************************

void Player::equipItem(ItemCard* aCard)
{

	if (equipIsAllowed(*aCard) )
	{
		vector<Card*>::iterator it;
		it = cardsInHand.begin();

		for (unsigned i = 0; i < cardsInHand.size(); i++)
		{
			if ((*cardsInHand[i]).title == (*aCard).title)
			{
				//Erase the i-th element
				cardsInHand.erase(cardsInHand.begin() + i);
			}
		}

		equippedCards.push_back(aCard);
	}
	
}

void Player::loseItem(ItemCard* aCard)
{
	//TODO: FILL IN THIS FUNCTION
}

void Player::playCard(Card* aCard)
{
	//action will be different based on CardType
	//TODO: FILL IN THIS FUNCTION
}

void Player::receiveCard(Card* aCard)
{
	cardsInHand.push_back(aCard);
}

void Player::discardCard(Card* aCard)
{
	//TODO: FILL IN THIS FUNCTION
}
void Player::askForHelp()
{
	//TODO: FILL IN THIS FUNCTION
}

string Player::printCardsInHand()
{
	string result = "";
	for (unsigned i = 0; i < cardsInHand.size(); i++)
	{
		result += (*cardsInHand[i]).print();
	}
	return result;
}


//***************     PRIVATE FUNCTIONS     ************************

void Player::goUpLevel()
{
	//Don't go higher than level 10
	if (!this->level == MAX_LEVEL)
		level++;
}

void Player::goDownLevel()
{
	//Don't go lower than level 1
	if (!this->level == MIN_LEVEL)
		level--;
}

bool Player::equipIsAllowed(const ItemCard &aCard)
{
	//TODO: TEST THIS FUNCTION
	switch (aCard.restrictionKey)
	{
	case Card::RestrictionKey::NOT_USABLE_BY_CLASS:
		if ((isSuperMunchkin() && (class2 == Card::ClassType::NO_CLASS)) ||		//Yes SuperMunchkin & Only one class
			(!(class1 == aCard.classRestriction || class2 == aCard.classRestriction)))	//Neither class is Restricted Class
			return true;
		break;
	case Card::RestrictionKey::NOT_USABLE_BY_RACE:
		if ((isHalfBreed() && (race2 == Card::RaceType::NO_RACE)) ||		//Yes HalfBreed & Only one race
			(!(race1 == aCard.raceRestriction || race2 == aCard.raceRestriction)))	//Neither race is Restricted race
			return true;
		break;
	case Card::RestrictionKey::NOT_USABLE_BY_GENDER:
		if (gender != aCard.genderRestriction)
			return true;
		break;
	case Card::RestrictionKey::USABLE_BY_CLASS_ONLY:
		if (class1 == aCard.classRestriction || class2 == aCard.classRestriction)
			return true;
		break;
	case Card::RestrictionKey::USABLE_BY_RACE_ONLY:
		if (race1 == aCard.raceRestriction || race2 == aCard.raceRestriction)
			return true;
		break;
	case Card::RestrictionKey::USABLE_BY_GENDER_ONLY:
		if (gender == aCard.genderRestriction)
			return true;
		break;
	case Card::RestrictionKey::NO_RESTRICTION:
		return true;
		break;
	default:
		return false;
		break;
	}

	return false;
}
