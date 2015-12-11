#ifndef HUMAN_H
#define HUMAN_H
#pragma once
#include "Player.h"

class Human :
	public Player
{
public:
	Human();
	Human(string n, Card::Gender g);

	void beginTurn(Game &currentGame);
	void enterBattlePhase(Game &currentGame, MonsterCard *monster);
	void enterDecidingPhase(Game &currentGame);
	void enterCharityPhase(Game &currentGame);
	void beefMeUp(int &playerStrength);
	bool playerWillMod(const int playerLevel, const int modsNeeded);

};

#endif