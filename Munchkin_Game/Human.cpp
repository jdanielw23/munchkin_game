#include "Human.h"

Human::Human()
{
}

// Constructor for an AI player that accepts arguments
Human::Human(string n, Card::Gender g) : Human()
{
	name = n;
	gender = g;
}

void Human::beginTurn(Game &currentGame)
{
	setTurnPhase(TurnPhase::EQUIPPING);
	bool doneEquipping = false;

	cout << "\tEquipping\n";	

	// Allow the player to keep equipping items until we are done
	while (!doneEquipping)
	{
		string chosenCard = "";

		// Print the equippable cards in the player's hand
		cout << endl << "Equippable Cards: " << endl;
		map<int, int> usableCards = printUsableCardsInHand();

		// Print the player's equipped cards
		cout << "\nEquipped Cards:" << endl;
		printEquippedCards();

		cout << "\nEnter the number of the card to equip (when finished, type \"done\"): " << endl;
		getline(cin, chosenCard);

		istringstream iss(chosenCard);
		int key = 0;
		iss >> key;

		//VALIDATE INPUT
		while (!(chosenCard == "done") && (usableCards.count(key) == 0))
		{
			cout << "Invalid Input. Try again: ";
			getline(cin, chosenCard);
			istringstream newISS(chosenCard);
			newISS.str(chosenCard);
			newISS >> key;
		}

		if (chosenCard == "done")
		{
			doneEquipping = true;
			break;
		}

		Card* selectedCard = cardsInHand[usableCards[key]];	//CHANGED THIS!! CHECK HERE IF TESTS FAIL
		equipCard(currentGame, selectedCard);
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

void Human::enterBattlePhase(Game &currentGame, MonsterCard *monster)
{
	setTurnPhase(TurnPhase::IN_BATTLE);
	int monsterStrength = getMonsterStrength(monster);
	string nameOfOneShotCard = "";
	bool found = false;

	cout << "\t\tPlayer " << to_string(getBattleStrength()) << "\t vs \t" << (*monster).title << " " << to_string(monsterStrength) << "\n";

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
			OneShotCard *oneShot = NULL;

			cout << "One Shot Cards:" << endl;
			for (unsigned i = 0; i < cardsInHand.size(); i++)
			{
				if ((*cardsInHand[i]).cardType == Card::CardType::ONE_SHOT)
				{
					cout << "\t" << (*cardsInHand[i]).title << endl;
				}
			};

			while (!found)
			{
				cout << "Pick a one shot card to use (or type \"none\" to skip): ";
				getline(cin, nameOfOneShotCard);
				found = false;

				for (unsigned i = 0; i < cardsInHand.size(); i++)
				{
					if ((*cardsInHand[i]).cardType == Card::CardType::ONE_SHOT && (*cardsInHand[i]).title == nameOfOneShotCard)
					{
						oneShot = dynamic_cast<OneShotCard*>(getCardsInHand()[i]);
						found = true;
					}
				};

				if (!found)
				{
					cout << "You don't have a One Shot card called \"" << nameOfOneShotCard << "\". Try again..." << endl;
				}
			};

			if (!(*oneShot).goUpLevel)
			{
				modAmount += (*oneShot).bonus;
				(*currentGame.getDiscardedTreasureCards()).addCard(discardCard(oneShot));

				cout << "\t\t\tUsed a One Shot: +" + to_string((*oneShot).bonus) + " for player.\n";
				if ((getBattleStrength() + modAmount) > monsterStrength)
					winBattle(currentGame, monster);
			}
		}
		else
			loseBattle(currentGame, monster);
	}
	else if ((getBattleStrength() + getModdableAmount()) > monsterStrength)
	{
		//****************************************************************************
		int modAmount = 0;
		string selection = "";
		bool winning = false;

		do
		{
			cout << "\tYou're Losing! Battle Strength: " + to_string(getBattleStrength() + modAmount) << endl;
			cout << "\tHere's what you can do: " << endl;
			map<int, int> usableCards = printUsableCardsInHand();

			cout << "Enter the number of the card to use or type \"lose\" to give up: ";
			getline(cin, selection);

			istringstream iss(selection);
			int key = 0;
			iss >> key;

			while (!(selection == "lose") && (usableCards.count(key) == 0))
			{
				cout << "Invalid Input. Try again: ";
				getline(cin, selection);
				istringstream newISS(selection);
				newISS.str(selection);
				newISS >> key;
			}
			if (selection == "lose")
			{
				loseBattle(currentGame, monster);
			}
			else
			{
				OneShotCard *oneShot = dynamic_cast<OneShotCard*>(cardsInHand[usableCards[key]]);
				if ((*oneShot).goUpLevel)
				{
					goUpLevel();
					cout << "\t\tPlayer Used Go Up a Level Card: " + (*oneShot).title << endl;
				}
				else
				{
					modAmount += (*oneShot).bonus;
					cout << "\t\tUsed a One Shot: +" + to_string((*oneShot).bonus) + " for player.\n";
				}

				(*currentGame.getDiscardedTreasureCards()).addCard(discardCard(oneShot));

				//Allow other players to mod again
				monsterStrength += currentGame.allowBattleMods(monsterStrength);

				if ((getBattleStrength() + modAmount) > monsterStrength)
				{
					winning = true;
					winBattle(currentGame, monster);
					break;
				}
			}
		} while (!winning);
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	}
	else
	{
		loseBattle(currentGame, monster);
	};

	//Discard Monster Card into discarded door cards;
	(*currentGame.getDiscardedDoorCards()).addCard(currentGame.getCardInPlay());

}

void Human::enterDecidingPhase(Game &currentGame)
{
	setTurnPhase(TurnPhase::DECIDING);
	string selection = "";

	cout << "\t****   DECIDING PHASE   ****" << endl;
	cout << "Available Monsters: " << endl;
	map<int, int> usableMonsters = printUsableCardsInHand();
	cout << endl;

	cout << "Available One Shot Cards: " << endl;
	printOneShotCards();
	cout << endl;

	cout << "My Strength: " + to_string(getBattleStrength()) << endl;

	cout << "Enter the number of the monster to battle or type \"loot\" to Loot the Room." << endl;
	cout << "Make your choice: ";
	getline(cin, selection);

	istringstream iss(selection);
	int key = 0;
	iss >> key;

	while (!(selection == "loot") && (usableMonsters.count(key) == 0))
	{
		cout << "Invalid Input. Try again: ";
		getline(cin, selection);
		istringstream newISS(selection);
		newISS.str(selection);
		newISS >> key;
	}

	if (selection == "loot")
	{
		if ((*currentGame.getDoorDeck()).isEmpty())
			currentGame.resetDoorDeck();
		Card *cardReceived = (*currentGame.getDoorDeck()).dealCard();
		cardsInHand.push_back(cardReceived);

		cout << "\t****   LOOT THE ROOM   ****\n";
		cout << "\t\tCard Received: " + (*cardReceived).print() << endl;
		enterCharityPhase(currentGame);
	}
	else
	{
		cout << "\t****   LOOK FOR TROUBLE   ****\n";
		MonsterCard *monster = dynamic_cast<MonsterCard*>(cardsInHand[usableMonsters[key]]);
		discardCard(monster);
		currentGame.setCardInPlay(monster);
		enterBattlePhase(currentGame, monster);
	}
}

void Human::enterCharityPhase(Game &currentGame)
{
	setTurnPhase(TurnPhase::CHARITY);

	cout << "\t****   CHARITY PHASE   ****\n";
	cout << "\t\tAUTO CHARITY\n";

	unsigned maxCards = 5;
	if (race1 == Card::RaceType::DWARF || race2 == Card::RaceType::DWARF)
		maxCards = 6;

	//************** CHANGE THIS TO GIVE PLAYER OPTION LATER  ***************
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
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	setTurnPhase(TurnPhase::WAITING);
	currentGame.setGameIsOver(currentGame.isGameOver());
}

void Human::beefMeUp(int &playerStrength)
{

}

bool Human::playerWillMod(const int playerLevel, const int modsNeeded)
{
	string decision = "";
	if (getModdableAmount() < modsNeeded)
		return false;
	else
	{
		cout << "One Shot Cards: " << endl;
		printOneShotCards();
		cout << endl;

		cout << "Would you like to add mods? (y/n): ";
		getline(cin, decision);

		if (decision == "y")
			return true;
		else
			return false;
	}
}