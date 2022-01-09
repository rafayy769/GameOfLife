#pragma once

const int MAX_ITEMS = 500;

enum Items
{
	iSWORD,
	iWATER,
	iSHIELD,
	iKEY
};

class Player
{
private:
	int mPoints;
	int mGoldCoins;
	int mSilverCoins;
	int movesToSkip;

	bool cellLocked;
	int nSwordUsed;
	int nWaterUsed;
	int nShieldUsed;

	int nItems;
	Items inventoryBox[MAX_ITEMS];
public:
	Player();
	//Player(int, int, int);

	int getPoints();
	int getGoldCoins();
	int getSilverCoins();
	int getMovesToSkip();

	//int getTotalPoints() { mTotalPoints = mPoints + mGoldCoins * 10 + mSilverCoins * 5; return mTotalPoints; }
	
	int getSwordUse() { return nSwordUsed; }
	void incrementSwordUse() { nSwordUsed++; }

	int getWaterUse() { return nWaterUsed; }
	void incrementWaterUse() { nWaterUsed++; }

	int getShieldUse() { return nShieldUsed; }
	void incrementShieldUse() { nShieldUsed++; }

	bool getLockedStatus() { return cellLocked; }
	void setLockedStatus(bool st) { cellLocked = st; }

	//void setTotalPoints(int s) { mTotalPoints = s; }

	void setSwordUse(int s) { nSwordUsed = s; }
	void setWaterUse(int s) { nWaterUsed = s; }
	void setShieldUse(int s) { nShieldUsed = s; }

	void setPoints(int);
	void setGoldCoins(int);
	void setSilverCoins(int);
	void setMovesToSkip(int);
	void setnItems(int n) { nItems = n; }
	void decrementMovesToSkip();

	void resetVariables() { cellLocked = false; movesToSkip = 0; nSwordUsed = 0; nWaterUsed = 0; nShieldUsed = 0; nWaterUsed = 0; nShieldUsed = 0; }

	int getnItems() { return nItems; }
	Items* getInventory()
	{
		return inventoryBox;
	}
	void addToInventory(Items item);
	void removeFromInventory(Items item);
};