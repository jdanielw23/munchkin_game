/**
* EGR 327 Project	CBU
* Constants.h			Header File for the Constants class
* Created 10-29-15	JDW
* This class contains the information for each deck of cards
* REVISION HISTORY:
*
*/

#pragma once
#include <vector>
#include <string>
#include "Card.h"

using namespace std;

class Constants
{
public:
	const string CLASSES[5] = { "Thief","Cleric","Wizard","Warrior","None" };
	const string RACES[4] = { "Hlflng","Elf","Dwarf","None" };
	const string GENDERS[2] = { "Male","Female" };
	const string SLOTS[5] = { "One Hand","Two Hands","Headgear","Footgear","Armor" };

};

