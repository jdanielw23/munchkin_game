

#ifndef AI_H
#define AI_H
#pragma once
#include "Player.h"

class AI :
	public Player
{
public:
	AI();
	AI(string n, Card::Gender g);

	map<int, bool> goingToMod;

	void beginTurn(Game &currentGame);
	void enterBattlePhase(Game &currentGame, MonsterCard *monster);
	void enterDecidingPhase(Game &currentGame);
	void enterCharityPhase(Game &currentGame);
	void beefMeUp(int &playerStrength) {};
	bool playerWillMod(const int playerLevel, const int modsNeeded);

};

#endif