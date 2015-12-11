#include "AI.h"



AI::AI() : Player()
{
	bool midRangeMod = false;
	if ((rand() % 100) < 50)
		midRangeMod = true;

	goingToMod[1] = false;
	goingToMod[2] = false;
	goingToMod[3] = false;
	goingToMod[4] = midRangeMod;
	goingToMod[5] = midRangeMod;
	goingToMod[6] = midRangeMod;
	goingToMod[7] = true;
	goingToMod[8] = true;
	goingToMod[9] = true;

}

// Constructor for an AI player that accepts arguments
AI::AI(string n, Card::Gender g) : AI()
{
	name = n;
	gender = g;
}

void AI::beginTurn(Game &currentGame)
{
	setTurnPhase(TurnPhase::EQUIPPING);

	cout << "\tEquipping\n";

	//In Equipping Phase; if we can equip an item, than do it for all card types
	for (unsigned i = 0; i < cardsInHand.size(); i++)
	{
		equipCard(currentGame, cardsInHand[i]);
	}

	//Bust down the door
	Card *currentCard = currentGame.bustDownDoor();

	if ((*currentCard).cardType == Card::CardType::MONSTER)
	{
		MonsterCard *monster = dynamic_cast<MonsterCard*>(currentCard);

		cout << "\tEntering battle with " + (*monster).title + "\n";
		enterBattlePhase(currentGame, monster);
	}
	else
	{
		cardsInHand.push_back(currentCard);	//Add card to player's hand

		cout << "\tNot a monster.\n";
		enterDecidingPhase(currentGame);
	}

}

void AI::enterBattlePhase(Game &currentGame, MonsterCard *monster)
{
	setTurnPhase(TurnPhase::IN_BATTLE);

	int monsterStrength = getMonsterStrength(monster);

	cout << "\t\tPlayer " + to_string(getBattleStrength()) + "\t vs \t" + (*monster).title + " " + to_string(monsterStrength) + "\n";

	if (getBattleStrength() > monsterStrength)
	{
		monsterStrength += currentGame.allowBattleMods(monsterStrength);

		if (getBattleStrength() > monsterStrength)
		{
			winBattle(currentGame, monster);
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

						cout << "\t\t\tUsed a One Shot: +" + to_string((*oneShot).bonus) + " for player.\n";
						if ((getBattleStrength() + modAmount) > monsterStrength)
							break;
					}
				}
			}
			winBattle(currentGame, monster);
		}
		else
			loseBattle(currentGame, monster);
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

					cout << "\t\t\tUsed a One Shot: +" + to_string((*oneShot).bonus) + " for player.\n";
					if ((getBattleStrength() + modAmount) > monsterStrength)
						break;
				}
			}
		}
		winBattle(currentGame, monster);
	}
	else
		loseBattle(currentGame, monster);

	//Discard Monster Card into discarded door cards;
	(*currentGame.getDiscardedDoorCards()).addCard(currentGame.getCardInPlay());

}

void AI::enterDecidingPhase(Game &currentGame)
{
	setTurnPhase(TurnPhase::DECIDING);

	bool lookForTrouble = false;

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

			cout << "\t****   LOOT THE ROOM   ****\n";
			enterCharityPhase(currentGame);
		}
		else
		{
			MonsterCard *monster = dynamic_cast<MonsterCard*>(currentGame.getCardInPlay());

			cout << "\t****   LOOK FOR TROUBLE   ****\n";
			enterBattlePhase(currentGame, monster);
		}
	
}

void AI::enterCharityPhase(Game &currentGame)
{
	setTurnPhase(TurnPhase::CHARITY);

	cout << "\t****   CHARITY PHASE   ****\n";
	cout << "\t\tAUTO CHARITY\n";

	unsigned maxCards = 5;
	if (race1 == Card::RaceType::DWARF || race2 == Card::RaceType::DWARF)
		maxCards = 6;

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

	setTurnPhase(TurnPhase::WAITING);
	currentGame.setGameIsOver(currentGame.isGameOver());
}

bool AI::playerWillMod(const int playerLevel, const int modsNeeded)
{
	if (getModdableAmount() < modsNeeded)
		return false;
	else 
		return goingToMod[playerLevel];
}