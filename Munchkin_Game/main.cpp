/**
* EGR 327 Project	CBU
* main.cpp			Source File for the main window
* Created 11-01-15	RSR
* This is user interface window.
* REVISION HISTORY:
* 11-01-15			RSR		Created File
* 12-06-15			RSR		Window was more work than worth, defaulted back to console window.

*/

#include <iostream>
#include <sstream>
#include <cstdio>
#include <ctime>
#include "Game.h"
#include "Deck.h"
#include "Tester.h"

using namespace std;

// Name of Window class
int main()
{
	int numPlayers;

	// Declare our game class
	Game MunchkinGame;

	MunchkinGame.beginGame();
	cout << MunchkinGame.playGame();

	cout << "GAME ENDED" << endl;

	int k;
	cin >> k;

	return 0;
};