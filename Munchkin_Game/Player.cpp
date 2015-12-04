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

//FOR TESTING
string Player::printCardsInHand()
{
	string result = "";
	for (unsigned i = 0; i < cardsInHand.size(); i++)
	{
		result += (*cardsInHand[i]).print();
	}
	return result;
}

void Player::beginTurn(Game &currentGame, string &output)
{
	setTurnPhase(TurnPhase::EQUIPPING);

	//FOR TESTING
	output += "\tEquipping\n";

	if (playerType == PlayerType::AI)
	{
		//In Equipping Phase
		for (unsigned i = 0; i < cardsInHand.size(); i++)
		{
			if ((*cardsInHand[i]).cardType == Card::CardType::ITEM)
			{
				ItemCard *item = dynamic_cast<ItemCard*>(cardsInHand[i]);
				if (equipItemIsAllowed(*item))
				{
					equipItem(item);
					output += "\t\tEquipped Item: " + (*item).title + " " + to_string((*item).bonus) + "\n";
				}
			}
			else if ((*cardsInHand[i]).cardType == Card::CardType::CLASS)
			{
				ClassCard *classCard = dynamic_cast<ClassCard*>(cardsInHand[i]);
				if (equipClassIsAllowed(*classCard))
				{
					equipClass(classCard);
					output += "\t\tEquipped Class: " + (*classCard).title + "\n";
				}
			}
			else if ((*cardsInHand[i]).cardType == Card::CardType::RACE)
			{
				RaceCard *raceCard = dynamic_cast<RaceCard*>(cardsInHand[i]);
				if (equipRaceIsAllowed(*raceCard))
				{
					equipRace(raceCard);
					output += "\t\tEquipped Race: " + (*raceCard).title + "\n";
				}
			}
			else if ((*cardsInHand[i]).cardType == Card::CardType::ONE_SHOT)
			{
				OneShotCard *oneShot = dynamic_cast<OneShotCard*>(cardsInHand[i]);
				if ((*oneShot).goUpLevel)
				{
					goUpLevel();
					discardCard(oneShot);
					output += "\t\tUse Go Up A Level Card: " + (*oneShot).title + "\n";
				}
			}
		}

		//Bust down the door
		Card *currentCard = currentGame.bustDownDoor();
		output += "\t****   BUST DOWN THE DOOR   ****\n";

		if ((*currentCard).cardType == Card::CardType::MONSTER)
		{
			MonsterCard *monster = dynamic_cast<MonsterCard*>(currentCard);

			output += "\tEntering battle with " + (*monster).title + "\n";
			enterBattlePhase(currentGame, monster, output);
		}
		else
		{
			cardsInHand.push_back(currentCard);	//Add card to player's hand

			output += "\tNot a monster.\n";
			enterDecidingPhase(currentGame, output);
		}
	}
}

void Player::enterBattlePhase(Game &currentGame, MonsterCard *monster, string &output)
{
	setTurnPhase(TurnPhase::IN_BATTLE);
	if (playerType == PlayerType::AI)
	{
		int monsterStrength = getMonsterStrength(monster);

		output += "\t\tPlayer " + to_string(getBattleStrength()) + "\t vs \t" + (*monster).title + " " + to_string(monsterStrength) + "\n";

		if (getBattleStrength() > monsterStrength)
		{
			monsterStrength += currentGame.allowBattleMods(monsterStrength, output);

			if (getBattleStrength() > monsterStrength)
				winBattle(currentGame, monster, output);
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

						if (!(*oneShot).goUpLevel)
						{
							modAmount += (*oneShot).bonus;
							(*currentGame.getDiscardedTreasureCards()).addCard(discardCard(oneShot));
							j -= 1;	//Repair index if one card was removed. This might not work correctly.**********************

							output += "\t\t\tUsed a One Shot: +" + to_string((*oneShot).bonus) + " for player.\n";
							if ((getBattleStrength() + modAmount) > monsterStrength)
								break;
						}
					}
				}
				winBattle(currentGame, monster, output);
			}
			else
				loseBattle(currentGame, monster, output);
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

					if (!(*oneShot).goUpLevel)
					{
						modAmount += (*oneShot).bonus;
						(*currentGame.getDiscardedTreasureCards()).addCard(discardCard(oneShot));
						j -= 1;	//Repair index if one card was removed. This might not work correctly.**********************

						output += "\t\t\tUsed a One Shot: +" + to_string((*oneShot).bonus) + " for player.\n";
						if ((getBattleStrength() + modAmount) > monsterStrength)
							break;
					}
				}
			}
			winBattle(currentGame, monster, output);
		}
		else
			loseBattle(currentGame, monster, output);
	}
}

void Player::enterDecidingPhase(Game &currentGame, string &output)
{
	setTurnPhase(TurnPhase::DECIDING);

	bool lookForTrouble = false;

	if (playerType == PlayerType::AI)
	{
		//If we can, look for trouble
		for (unsigned i = 0; i < cardsInHand.size(); i++)
		{
			if ((*cardsInHand[i]).cardType == Card::CardType::MONSTER)
			{
				MonsterCard *monster = dynamic_cast<MonsterCard*>(cardsInHand[i]);
				if (getMonsterStrength(monster) < getBattleStrength())
				{
					cardsInHand.erase(cardsInHand.begin() + i);
					currentGame.setCardInPlay(monster);
					lookForTrouble = true;
					break;
				}
			}
		}
		//Otherwise, Loot the room
		if (!lookForTrouble)
		{
			if ((*currentGame.getDoorDeck()).isEmpty())
				currentGame.resetDoorDeck();
			cardsInHand.push_back((*currentGame.getDoorDeck()).dealCard());

			output += "\t****   LOOT THE ROOM   ****\n";
			enterCharityPhase(currentGame, output);
		}
		else
		{
			MonsterCard *monster = dynamic_cast<MonsterCard*>(currentGame.getCardInPlay());

			output += "\t****   LOOK FOR TROUBLE   ****\n";
			enterBattlePhase(currentGame, monster, output);
		}
	}

}

void Player::enterCharityPhase(Game &currentGame, string &output)
{
	setTurnPhase(TurnPhase::CHARITY);

	int maxCards = 5;
	if (race1 == Card::RaceType::DWARF || race2 == Card::RaceType::DWARF)
		maxCards = 6;
	
	if (playerType == PlayerType::AI)
	{
		while (cardsInHand.size() > maxCards)
		{
			Card *card = cardsInHand.front();
			cardsInHand.erase(cardsInHand.begin());
			if ((*card).cardType == Card::CardType::ITEM ||
				(*card).cardType == Card::CardType::ONE_SHOT)
				(*currentGame.getDiscardedTreasureCards()).addCard(card);
			else
				(*currentGame.getDiscardedDoorCards()).addCard(card);
		}
	}

	setTurnPhase(TurnPhase::WAITING);
	currentGame.setGameIsOver(currentGame.isGameOver());
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

void Player::loseBattle(Game &currentGame, MonsterCard *monster, string &output)
{
	(*currentGame.getDiscardedDoorCards()).addCard(monster);

	output += "\t\tPLAYER LOSES!\n";

	bool altLoseLevel = (*monster).altLoseLevel;
	switch ((*monster).badStuff)
	{
	case Card::BadStuff::LOSE_FOOTGEAR:
		if(!loseItem(currentGame, Card::SlotType::FOOTGEAR) && altLoseLevel)
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
		break;
	case Card::BadStuff::LOSE_SMALL_ITEM:
		break;
	case Card::BadStuff::LOSE_HAND:
		break;
	case Card::BadStuff::LOSE_PLUS_THREE_ITEM:
		break;
	case Card::BadStuff::LOSE_ONE_SHOT:
		break;
	case Card::BadStuff::LOSE_HIGHEST_BONUS:
		break;
	case Card::BadStuff::LOSE_TWO_LEVELS:
		goDownLevel();
		goDownLevel();
		break;
	case Card::BadStuff::LOSE_HAND_ITEMS:
		break;
	case Card::BadStuff::LOSE_HAND_ITEM:
		break;
	case Card::BadStuff::LOSE_ARMOR:
		if(!loseItem(currentGame, Card::SlotType::ARMOR) && altLoseLevel)
			goDownLevel();
		break;
	case Card::BadStuff::LOSE_BIG_ITEM:
		break;
	case Card::BadStuff::LOSE_TWO_ITEMS:
		break;
	default:
		break;
	}

	enterCharityPhase(currentGame, output);
}

void Player::winBattle(Game &currentGame, MonsterCard *monster, string &output)
{
	(*currentGame.getDiscardedDoorCards()).addCard(monster);

	output += "\t\tPLAYER WINS!\n";

	for (int i = 0; i < (*monster).numLevels; i++)
	{
		output += "\t\t\tGo Up A Level\n";
		goUpLevel();
	}

	for (int i = 0; i < (*monster).numTreasures; i++)
	{
		if ((*currentGame.getTreasureDeck()).isEmpty())
			currentGame.resetTreasureDeck();

		cardsInHand.push_back((*currentGame.getTreasureDeck()).dealCard());
	}

	enterCharityPhase(currentGame, output);
}

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

bool Player::operator!=(const Player &p)
{
	return (name != p.name);
}


//***************     PRIVATE FUNCTIONS     ************************

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