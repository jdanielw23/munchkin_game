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
	turnPhase = TurnPhase::WAITING;

	//Initialize Slots
	equippedSlots[Card::SlotType::ONE_HAND] = false;
	equippedSlots[Card::SlotType::TWO_HANDS] = false;
	equippedSlots[Card::SlotType::HEADGEAR] = false;
	equippedSlots[Card::SlotType::FOOTGEAR] = false;
	equippedSlots[Card::SlotType::ARMOR] = false;
}

Player::Player(string n, PlayerType p, Card::Gender g) : Player()
{
	name = n;
	playerType = p;
	gender = g;
}

Player::~Player()
{
}

//***************     PUBLIC FUNCTIONS     ************************

void Player::equipItem(ItemCard* aCard)
{

	if (equipItemIsAllowed(*aCard) )
	{

		for (unsigned i = 0; i < cardsInHand.size(); i++)
		{
			if ((*cardsInHand[i]).title == (*aCard).title)
			{
				//Remove card from hand
				cardsInHand.erase(cardsInHand.begin() + i);	//Erase the i-th element
				break;
			}
		}

		//Add card to equipped cards
		equippedCards.push_back(aCard);

		gear += (*aCard).bonus;		//Add bonus to player's gear

		if ((*aCard).bigItem)
			bHasBigItem = true;		//Check to see if it is a big item

		if ((*aCard).slotType != Card::SlotType::NONE)
			equippedSlots[(*aCard).slotType] = true;	//Occupy the correct slot
	}
	
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

string Player::printCardsInHand()
{
	string result = "";
	for (unsigned i = 0; i < cardsInHand.size(); i++)
	{
		result += (*cardsInHand[i]).print();
	}
	return result;
}

void Player::beginTurn(Game &currentGame)
{
	setTurnPhase(TurnPhase::EQUIPPING);

	if (playerType == PlayerType::AI)
	{
		//In Equipping Phase
		for (unsigned i = 0; i < cardsInHand.size(); i++)
		{
			if ((*cardsInHand[i]).cardType == Card::CardType::ITEM)
			{
				ItemCard *item = dynamic_cast<ItemCard*>(cardsInHand[i]);
				if (equipItemIsAllowed(*item))
					equipItem(item);
			}
			else if ((*cardsInHand[i]).cardType == Card::CardType::CLASS)
			{
				ClassCard *classCard = dynamic_cast<ClassCard*>(cardsInHand[i]);
				if (equipClassIsAllowed(*classCard))
					equipClass(classCard);
			}
			else if ((*cardsInHand[i]).cardType == Card::CardType::RACE)
			{
				RaceCard *raceCard = dynamic_cast<RaceCard*>(cardsInHand[i]);
				if (equipRaceIsAllowed(*raceCard))
					equipRace(raceCard);
			}
		}

		//Bust down the door
		Card *currentCard = currentGame.bustDownDoor();
		if ((*currentCard).cardType == Card::CardType::MONSTER)
		{
			MonsterCard *monster = dynamic_cast<MonsterCard*>(currentCard);
			enterBattlePhase(currentGame, monster);
		}
		else
		{
			cardsInHand.push_back(currentCard);	//Add card to player's hand
			enterDecidingPhase(currentGame);
		}
	}
}

void Player::enterBattlePhase(Game &currentGame, MonsterCard *monster)
{
	setTurnPhase(TurnPhase::IN_BATTLE);
	if (playerType == PlayerType::AI)
	{
		int monsterStrength = getMonsterStrength(monster);

		if (getBattleStrength() > monsterStrength)
		{
			monsterStrength += currentGame.allowBattleMods(monsterStrength);
			if (getBattleStrength() > monsterStrength)
				winBattle();
			else if ((getBattleStrength() + getModdableAmount()) > monsterStrength)
			{
				int modAmount = 0;
				//Play his own cards to beef himself up
				for (unsigned j = 0; j < getCardsInHand().size(); j++)
				{
					if ((*getCardsInHand()[j]).cardType == Card::CardType::ONE_SHOT)
					{
						//Discard each one shot card that the player just used
						OneShotCard *oneShot = dynamic_cast<OneShotCard*>(getCardsInHand()[j]);
						modAmount += (*oneShot).bonus;
						(*currentGame.getDiscardedTreasureCards()).addCard(discardCard(oneShot));
						j -= 1;	//Repair index if one card was removed. This might not work correctly.**********************
						if ((getBattleStrength() + modAmount) > monsterStrength)
							break;
					}
				}
			}
			else
				loseBattle();
		}
		else if ((getBattleStrength() + getModdableAmount()) > monsterStrength)
		{
			int modAmount = 0;
			//Play his own cards to beef himself up
			for (unsigned j = 0; j < getCardsInHand().size(); j++)
			{
				if ((*getCardsInHand()[j]).cardType == Card::CardType::ONE_SHOT)
				{
					//Discard each one shot card that the player just used
					OneShotCard *oneShot = dynamic_cast<OneShotCard*>(getCardsInHand()[j]);
					modAmount += (*oneShot).bonus;
					(*currentGame.getDiscardedTreasureCards()).addCard(discardCard(oneShot));
					j -= 1;	//Repair index if one card was removed. This might not work correctly.**********************
					if ((getBattleStrength() + modAmount) > monsterStrength)
						break;
				}
			}
		}
		else
			loseBattle();
	}
}

void Player::enterDecidingPhase(Game &currentGame)
{
	setTurnPhase(TurnPhase::DECIDING);
	if (playerType == PlayerType::AI)
	{

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

void Player::loseBattle()
{

}

bool Player::winBattle()
{
	return true;
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

bool Player::equipItemIsAllowed(const ItemCard &aCard)
{
	//TEST FOR BIG ITEM - Dwarves can have any number of big items
	if (aCard.bigItem && hasBigItem() && race1 != Card::RaceType::DWARF && race2 != Card::RaceType::DWARF)
		return false;

	//TEST FOR SLOT FILLED
	if (equippedSlots[aCard.slotType])
		return false;

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