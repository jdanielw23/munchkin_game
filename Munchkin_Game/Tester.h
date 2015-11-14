/**
* EGR 327 Project	CBU
* Tester.h			Header File for the Test class
* Created 11-13-15	JDW
* This class run tests on all of the functions and writes output to a file
*
* REVISION HISTORY:
*
*/

#include <iostream>
#include <fstream>
#include <string>
#pragma once

using namespace std;

class Tester
{
private:
	//PRIVATE VARIABLES
	const string PASS = "Pass";
	const string FAIL = "FAILED";
	const string ELLIPSES = "....................";

	ofstream myFile;

	//PRIVATE FUNCTIONS
	string printDoorDeckTest();
	string printTreasureDeckTest();
	bool dealCardsFromDeckTest();

public:
	Tester();
	~Tester();

	void runTests();
};

