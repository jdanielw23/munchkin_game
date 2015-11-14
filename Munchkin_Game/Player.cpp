/**
* EGR 327 Project	CBU
* Player.cpp		Implementation File for the Player class
* Created 10-29-15	JDW
* This class contains the main logic for a player in the game.
* REVISION HISTORY:
*
*/

#include "Player.h"



Player::Player()
{
	level = 1;
}


Player::~Player()
{
}

//***************     PUBLIC FUNCTIONS     ************************

void Player::playCard(Card aCard)
{
	//action will be different based on CardType
}
void Player::discardCard(Card aCard)
{

}
void Player::askForHelp()
{

}


//***************     PRIVATE FUNCTIONS     ************************

void Player::equipItem(ItemCard aCard)
{

}
void Player::loseItem(ItemCard aCard)
{

}
void Player::goUpLevel()
{
	//Don't go higher than level 10
}

void Player::goDownLevel()
{
	//Don't go lower than level 1
}
