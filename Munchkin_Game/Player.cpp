/**
* EGR 327 Project	CBU
* Player.cpp		Implementation File for the Player class
* Created 10-29-15	JDW
* This class contains the main logic for a player in the game.
* REVISION HISTORY:
*/

#include "Player.h"

//TODO: Fill in methods for loseBattle() bad stuff

Player::Player()
{
	// Initialize data
	level = MIN_LEVEL;
	gear = 0;
	class1 = Card::ClassType::NO_CLASS;
	class2 = Card::ClassType::NO_CLASS;
	race1 = Card::RaceType::NO_RACE;
	race2 = Card::RaceType::NO_RACE;
	bIsSuperMunchkin = false;
	bIsHalfBreed = false;
	turnPhase = TurnPhase::WAITING;

	//Initialize Slots
	equippedSlots[Card::SlotType::ONE_HAND] = false;
	equippedSlots[Card::SlotType::TWO_HANDS] = false;
	equippedSlots[Card::SlotType::HEADGEAR] = false;
	equippedSlots[Card::SlotType::FOOTGEAR] = false;
	equippedSlots[Card::SlotType::ARMOR] = false;
}



//***************     PUBLIC FUNCTIONS     ************************

void Player::loseBattle(Game &currentGame, MonsterCard *monster)
{
	(*currentGame.getDiscardedDoorCards()).addCard(monster);

	cout << "\t\tPLAYER LOSES!\n";

	bool altLoseLevel = (*monster).altLoseLevel;
	switch ((*monster).badStuff)
	{
	case Card::BadStuff::LOSE_FOOTGEAR:
		if (!loseItem(currentGame, Card::SlotType::FOOTGEAR) && altLoseLevel)
			goDownLevel();
		break;
	case Card::BadStuff::LOSE_HEADGEAR:
		if (!loseItem(currentGame, Card::SlotType::HEADGEAR) && altLoseLevel)
			goDownLevel();
		break;
	case Card::BadStuff::LOSE_LEVEL:
		goDownLevel();
		break;
	case Card::BadStuff::DEATH:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_SMALL_ITEM:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_HAND:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_PLUS_THREE_ITEM:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_ONE_SHOT:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_HIGHEST_BONUS:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_TWO_LEVELS:
		goDownLevel();
		goDownLevel();
		break;
	case Card::BadStuff::LOSE_HAND_ITEMS:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_HAND_ITEM:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_ARMOR:
		if (!loseItem(currentGame, Card::SlotType::ARMOR) && altLoseLevel)
			goDownLevel();
		break;
	case Card::BadStuff::LOSE_BIG_ITEM:
		//TODO: implement function
		break;
	case Card::BadStuff::LOSE_TWO_ITEMS:
		//TODO: implement function
		break;
	default:
		break;
	}

	enterCharityPhase(currentGame);
}

void Player::winBattle(Game &currentGame, MonsterCard *monster)
{
	(*currentGame.getDiscardedDoorCards()).addCard(monster);

	cout << "\t\tPLAYER WINS!\n";

	for (int i = 0; i < (*monster).numLevels; i++)
	{
		cout << "\t\t\tGo Up A Level\n";
		goUpLevel();
	}

	for (int i = 0; i < (*monster).numTreasures; i++)
	{
		if ((*currentGame.getTreasureDeck()).isEmpty())
			currentGame.resetTreasureDeck();

		cardsInHand.push_back((*currentGame.getTreasureDeck()).dealCard());
	}

	enterCharityPhase(currentGame);
}


void Player::equipItem(ItemCard* aCard)
{

	if (equipItemIsAllowed(*aCard))
	{
		discardCard(aCard);	//Remove card from hand

		//Add card to equipped cards
		equippedCards.push_back(aCard);

		gear += (*aCard).bonus;		//Add bonus to player's gear

		if ((*aCard).bigItem)
			bHasBigItem = true;		//Check to see if it is a big item

		if ((*aCard).slotType != Card::SlotType::NONE)
		{
			if ((*aCard).slotType == Card::SlotType::ONE_HAND && equippedSlots[Card::SlotType::ONE_HAND])
				equippedSlots[Card::SlotType::TWO_HANDS] = true;
			else
				equippedSlots[(*aCard).slotType] = true;	//Occupy the correct slot
		}
	}

}

void Player::equipClass(ClassCard* aCard)
{
	if (equipClassIsAllowed(*aCard))
	{
		for (unsigned i = 0; i < cardsInHand.size(); i++)
		{
			if ((*cardsInHand[i]).title == (*aCard).title)
			{
				//Remove card from hand
				cardsInHand.erase(cardsInHand.begin() + i);	//Erase the i-th element
			}
		}

		//Add card to equipped cards
		equippedCards.push_back(aCard);

		if (class1 == Card::ClassType::NO_CLASS)
			class1 = (*aCard).classType;
		else
			class2 = (*aCard).classType;		//Already checked to make sure player is super munchkin
	}
}

void Player::equipRace(RaceCard *aCard)
{
	if (equipRaceIsAllowed(*aCard))
	{
		for (unsigned i = 0; i < cardsInHand.size(); i++)
		{
			if ((*cardsInHand[i]).title == (*aCard).title)
			{
				//Remove card from hand
				cardsInHand.erase(cardsInHand.begin() + i);	//Erase the i-th element
			}
		}

		//Add card to equipped cards
		equippedCards.push_back(aCard);

		if (race1 == Card::RaceType::NO_RACE)
			race1 = (*aCard).raceType;
		else
			race2 = (*aCard).raceType;		//Already checked to make sure player is half-breed
	}
}

int Player::getMonsterStrength(MonsterCard *monster)
{
	int monsterStrength = (*monster).level;
	if (gender == (*monster).genderTypeForBonus)
		monsterStrength += (*monster).genderBonus;
	if (class1 == (*monster).classTypeForBonus || class2 == (*monster).classTypeForBonus)
		monsterStrength += (*monster).classBonus;
	if (race1 == (*monster).raceTypeForBonus || race2 == (*monster).raceTypeForBonus)
		monsterStrength += (*monster).raceBonus;

	return monsterStrength;
}

int Player::getModdableAmount()
{
	int ableToAdd = 0;
	//Determine how much player can modify
	for (unsigned i = 0; i < getCardsInHand().size(); i++)
	{
		if ((*getCardsInHand()[i]).cardType == Card::CardType::ONE_SHOT)
		{
			OneShotCard *oneShot = dynamic_cast<OneShotCard*>(getCardsInHand()[i]);
			ableToAdd += (*oneShot).bonus;
		}
	}
	return ableToAdd;
}

//BAD STUFF FUNCTIONS
bool Player::loseItem(Game &currentGame, Card::SlotType slot)
{
	for (unsigned i = 0; i < equippedCards.size(); i++)
	{
		if ((*equippedCards[i]).cardType == Card::CardType::ITEM)
		{
			ItemCard *item = dynamic_cast<ItemCard*>(equippedCards[i]);
			if ((*item).slotType == slot)
			{
				(*currentGame.getDiscardedTreasureCards()).addCard(item);	//add card to discard pile
				equippedCards.erase(equippedCards.begin() + i);		//Remove card from hand
				gear -= (*item).bonus;		//Remove bonus given by card
				equippedSlots[slot] = false;		//Unoccupy item slot
				return true;
			}
		}
	}
	return false;
}


void Player::receiveCard(Card* aCard)
{
	cardsInHand.push_back(aCard);
}

Card* Player::discardCard(Card* aCard)
{
	Card* discardedCard = aCard;
	for (unsigned i = 0; i < cardsInHand.size(); i++)
	{
		if ((*cardsInHand[i]).title == (*aCard).title)
		{
			//Remove card from hand
			discardedCard = cardsInHand[i];
			cardsInHand.erase(cardsInHand.begin() + i);	//Erase the i-th element
			break;
		}
	}

	return discardedCard;
}

bool Player::operator!=(const Player &p)
{
	return (name != p.name);
}

map<int,int> Player::printUsableCardsInHand()
{
	int itemNumber = 1;
	map<int, int> cards;	//Key: Line Number; Value: i

	for (unsigned i = 0; i < cardsInHand.size(); i++)
	{
		switch (turnPhase)
		{
		case TurnPhase::EQUIPPING:
		{
			if ((*cardsInHand[i]).cardType == Card::CardType::ITEM)
			{
				ItemCard *item = dynamic_cast<ItemCard*>(cardsInHand[i]);
				if (equipItemIsAllowed(*item))
				{
					cout << to_string(itemNumber) + ". " + (*cardsInHand[i]).print();
					cards[itemNumber] = i;
					itemNumber++;
				}
			}
			else if ((*cardsInHand[i]).cardType == Card::CardType::CLASS)
			{
				ClassCard *classCard = dynamic_cast<ClassCard*>(cardsInHand[i]);
				if (equipClassIsAllowed(*classCard))
				{
					cout << to_string(itemNumber) + ". " + (*cardsInHand[i]).print();
					cards[itemNumber] = i;
					itemNumber++;
				}
			}
			else if ((*cardsInHand[i]).cardType == Card::CardType::RACE)
			{
				RaceCard *raceCard = dynamic_cast<RaceCard*>(cardsInHand[i]);
				if (equipRaceIsAllowed(*raceCard))
				{
					cout << to_string(itemNumber) + ". " + (*cardsInHand[i]).print();
					cards[itemNumber] = i;
					itemNumber++;
				}
			}
			else if ((*cardsInHand[i]).cardType == Card::CardType::ONE_SHOT)
			{
				OneShotCard *oneShot = dynamic_cast<OneShotCard*>(cardsInHand[i]);
				if ((*oneShot).goUpLevel)
				{
					cout << to_string(itemNumber) + ". " + (*cardsInHand[i]).print();
					cards[itemNumber] = i;
					itemNumber++;
				}
			}
			break;
		}
		case TurnPhase::DECIDING:
		{
			if ((*cardsInHand[i]).cardType == Card::CardType::MONSTER)
			{
				cout << to_string(itemNumber) + ". " + (*cardsInHand[i]).print();
				cards[itemNumber] = i;
				itemNumber++;
			}
			break;
		}
		case TurnPhase::IN_BATTLE:
		{
			if ((*cardsInHand[i]).cardType == Card::CardType::ONE_SHOT)
			{
				cout << to_string(itemNumber) + ". " + (*cardsInHand[i]).print();
				cards[itemNumber] = i;
				itemNumber++;
			}
			break;
		}
		default:
		{
			cout << to_string(itemNumber) + ". " + (*cardsInHand[i]).print();
			cards[itemNumber] = i;
			itemNumber++;
			break;
		}
		}		
	}

	return cards;
}


//************************     PRIVATE FUNCTIONS     ************************

void Player::goUpLevel()
{
	//Don't go higher than level 10
	if (level < MAX_LEVEL)
		level++;
}

void Player::goDownLevel()
{
	//Don't go lower than level 1
	if (level > MIN_LEVEL)
		level--;
}

bool Player::equipItemIsAllowed(const ItemCard &aCard)
{
	//TEST FOR BIG ITEM - Dwarves can have any number of big items
	if (aCard.bigItem && hasBigItem() && race1 != Card::RaceType::DWARF && race2 != Card::RaceType::DWARF)
		return false;

	//TEST FOR SLOT FILLED
	if (aCard.slotType != Card::SlotType::NONE)
	{
		if (aCard.slotType == Card::SlotType::HEADGEAR || aCard.slotType == Card::SlotType::FOOTGEAR ||
			aCard.slotType == Card::SlotType::ARMOR)
		{
			if (equippedSlots[aCard.slotType])
				return false;
		}
		//Card is either OneHand or TwoHands
		else if (equippedSlots[Card::SlotType::TWO_HANDS] || (aCard.slotType == Card::SlotType::TWO_HANDS && equippedSlots[Card::SlotType::ONE_HAND]))
			return false;
	}

	//TEST FOR ITEM RESTRICTION
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

bool Player::equipClassIsAllowed(const ClassCard &aCard)
{
	if (class1 == Card::ClassType::NO_CLASS || 
		(class2 == Card::ClassType::NO_CLASS && isSuperMunchkin()))
		return true;
	else
		return false;
}

bool Player::equipRaceIsAllowed(const RaceCard &aCard)
{
	if (race1 == Card::RaceType::NO_RACE ||
		(race2 == Card::RaceType::NO_RACE && isHalfBreed()))
		return true;
	else
		return false;
}

void Player::printOneShotCards()
{
	for (unsigned i = 0; i < cardsInHand.size(); i++)
	{
		if ((*cardsInHand[i]).cardType == Card::CardType::ONE_SHOT)
		{
			cout << (*cardsInHand[i]).print();
		}
	}
}

void Player::printEquippedCards()
{
	if (equippedCards.size() == 0)
		cout << "(none)" << endl;
	else
	{
		for (unsigned i = 0; i < equippedCards.size(); i++)
		{
			cout << "\t" << (*equippedCards[i]).print();
		}
		cout << endl;
	}
}

void Player::equipCard(Game &currentGame, Card* card)
{
	switch ((*card).cardType)
	{
	case Card::CardType::ITEM:
	{
		ItemCard *item = dynamic_cast<ItemCard*>(card);
		equipItem(item);
		cout << "\t\tEquipped Item: " << (*item).print();
		break;
	}
	case Card::CardType::CLASS:
	{
		ClassCard *classCard = dynamic_cast<ClassCard*>(card);
		equipClass(classCard);
		cout << "\t\tEquipped Class: " << (*classCard).print();
		break;
	}
	case Card::CardType::RACE:
	{
		RaceCard *raceCard = dynamic_cast<RaceCard*>(card);
		equipRace(raceCard);
		cout << "\t\tEquipped Race: " << (*raceCard).print();
		break;
	}
	case Card::CardType::ONE_SHOT:
	{
		goUpLevel();
		(*currentGame.getDiscardedTreasureCards()).addCard(discardCard(card));
		cout << "\t\tUsed Go Up A Level Card: " << (*card).title << "\n";
		break;
	}
	default:
		break;
	}
}