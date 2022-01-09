#pragma once

#include <SFML/Graphics.hpp>
#include "Tile.h"
#include <string>

enum CellState
{
	EMPTY,
	GOAL,
	PLAYER1,
	PLAYER2,
	GOLD,
	SILVER,
	FIRE,
	SNAKE,
	GHOST,
	LION,
	LOCK
};

struct Cell
{
	CellState state;
	int playerState;
};

class Board
{
private:
	int dim;  //current dimension of the board, eg 5x5, 7x7 etc.
	Cell** boardArray;  //for the dynamic board array

	sf::Texture tgoldCoins;
	sf::Texture tsilverCoins;
	sf::Texture tSword;
	sf::Texture tShield;
	sf::Texture tWater;
	sf::Texture tKey;
	sf::Texture tFire;
	sf::Texture tSnake;
	sf::Texture tGhost;
	sf::Texture tLion;
	sf::Texture tLock;
	sf::Texture dummy;
	sf::Font font;

	sf::Text p1Text;
	sf::Text p2Text;

	Tile* tiles;
	friend class Game;
public:
	Board();
	~Board();

	void initializeBoard(int size);
	void printBoard();
	void deletePreviousBoard();
};

