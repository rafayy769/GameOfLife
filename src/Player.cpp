#include "Player.h"

Player::Player()
{
	nItems = 0;
	mPoints = 0;
	mGoldCoins = 0;
	mSilverCoins = 0;
	movesToSkip = 0;
	cellLocked = false;
	nWaterUsed = 0;
	nShieldUsed = 0;
	nSwordUsed = 0;
	nWaterUsed = 0;
	nShieldUsed = 0;
}

int Player::getPoints()
{
	return mPoints;
}

int Player::getGoldCoins()
{
	return mGoldCoins;
}

int Player::getSilverCoins()
{
	return mSilverCoins;
}

int Player::getMovesToSkip()
{
	return movesToSkip;
}

void Player::setPoints(int p)
{
	mPoints = p;
}
void Player::setGoldCoins(int p)
{
	mGoldCoins = p;
}
void Player::setSilverCoins(int p)
{
	mSilverCoins = p;
}

void Player::setMovesToSkip(int p)
{
	movesToSkip = p;
}

void Player::decrementMovesToSkip()
{
	movesToSkip--;
}

void Player::addToInventory(Items item)
{
	inventoryBox[nItems] = item;
	nItems++;
}

void Player::removeFromInventory(Items item)
{
	int ind = 0;
	for (int i = 0; i < nItems; i++)
	{
		if (inventoryBox[i] == item)
		{
			ind = i;
			break;
		}
	}
	for (int i = ind + 1; i < nItems; i++)
	{
		inventoryBox[i - 1] = inventoryBox[i];
	}
	nItems--;
}
