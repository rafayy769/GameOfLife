#include "Board.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Board::Board()
{
	srand(time(0));
	dim = 5;
	font.loadFromFile("Resources/joystix monospace.ttf");
	tgoldCoins.loadFromFile("Resources/gold.png");
	tsilverCoins.loadFromFile("Resources/silver.png");
	tFire.loadFromFile("Resources/Fire.png");
	tGhost.loadFromFile("Resources/ghost.png");
	tLock.loadFromFile("Resources/lock.png");
	tSnake.loadFromFile("Resources/Snake.png");
	tLion.loadFromFile("Resources/Lion.png");
	dummy.loadFromFile("Resources/Dummy.png");

	p1Text.setFont(font);
	p2Text.setFont(font);
	p1Text.setFillColor(sf::Color::Yellow);
	p2Text.setFillColor(sf::Color::Yellow);
	p1Text.setString("P1");
	p2Text.setString("P2");

	boardArray = new Cell * [dim];
	tiles = new Tile[dim * dim];
	for (int i = 0; i < dim; i++)
	{
		boardArray[i] = new Cell[dim];
	}
	initializeBoard(5);
}

void Board::initializeBoard(int size)
{
	dim = size;
	int mid = (dim * dim) / 2;
	tiles = new Tile[dim * dim];
	boardArray = new Cell * [dim];

	for (int i = 0; i < dim; i++)
	{
		boardArray[i] = new Cell[dim];
	}
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			boardArray[i][j].state = EMPTY; //initializing all board cells to empty
		}
	}
	int boardmid = dim / 2;
	boardArray[boardmid][boardmid].state = GOAL;
	boardArray[0][0].state = PLAYER2;
	boardArray[dim - 1][0].state = PLAYER1;

	int counter = 0;
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				if (counter == 0 || counter == mid || counter == (dim * dim) - dim)
				{
					counter++;
					continue;
					
				}
				else
				{
					int a = rand() % 4;
					int b = rand() % 2;
					//std::cout << "a : " << a << std::endl << "b : " << b << std::endl << std::endl;
					if (a == 1 && b == 0)
					{
						boardArray[i][j].state = GOLD;
						tiles[counter].sprite.setTexture(tgoldCoins);
					}
					else if (a == 1 && b == 1)
					{
						boardArray[i][j].state = SILVER;
						tiles[counter].sprite.setTexture(tsilverCoins);
					}
					counter++;
				}
			}
		}

	for (int j = 0; j < dim * dim; j++)
	{
		tiles[j].recTile.setSize(sf::Vector2f(650.0f / dim, 650.0f / dim));
		if (j < mid)
		{
			tiles[j].recTile.setFillColor(sf::Color{ 229,56,122 });
		}
		else if (j == mid)
		{
			tiles[j].recTile.setFillColor(sf::Color{ 71,215,71 });
		}
		else
		{
			tiles[j].recTile.setFillColor(sf::Color{ 0,120,255,255 });
		}
		tiles[j].recTile.setOutlineColor(sf::Color::Black);
		tiles[j].recTile.setOutlineThickness(1);
		tiles[j].text.setFont(font);
		if (j != mid)
		{
			std::string a = std::to_string(j + 1);
			tiles[j].text.setString(a);
		}
		else
		{
			tiles[j].text.setString("GOAL");
		}
		tiles[j].text.setCharacterSize(tiles[j].recTile.getSize().y * 0.25f);
	}
	counter = 0;
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			tiles[counter].recTile.setPosition((j * tiles[counter].recTile.getSize().y), (i * tiles[counter].recTile.getSize().x));  ///x,y
			tiles[counter].sprite.setPosition(j * tiles[counter].recTile.getSize().y + tiles[counter].recTile.getSize().y / 2.0f, i * tiles[counter].recTile.getSize().x + tiles[counter].recTile.getSize().x / 2.0f);
			tiles[counter].text.setPosition(j * tiles[counter].recTile.getSize().y, i * tiles[counter].recTile.getSize().x);
			counter++;
		}
	}
	p2Text.setPosition(0 * tiles[0].recTile.getSize().y + tiles[0].recTile.getSize().y / 2.0f,
					   0 * tiles[0].recTile.getSize().x + tiles[0].recTile.getSize().x / 2.0f);
	p1Text.setPosition(0 * tiles[0].recTile.getSize().y + tiles[0].recTile.getSize().y / 2.0f,
					   (dim-1) * tiles[dim-1].recTile.getSize().x + tiles[dim-1].recTile.getSize().x / 2.0f);
	
	printBoard();
}

void Board::printBoard()
{
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			if (boardArray[i][j].state == PLAYER1)
			{
				std::cout << "P1" << " ";
			}
			else if (boardArray[i][j].state == PLAYER2)
			{
				std::cout << "P2" << " ";
			}
			else if (boardArray[i][j].state == EMPTY)
			{
				std::cout << "empty" << " ";
			}
			else if (boardArray[i][j].state == GOLD)
			{
				std::cout << "Gold" << " ";
			}
			else if (boardArray[i][j].state == SILVER)
			{
				std::cout << "silver" << " ";
			}
			else if (boardArray[i][j].state == GOAL)
			{
				std::cout << "Goal" << " ";
			}
			else if (boardArray[i][j].state == FIRE)
			{
				std::cout << "Fire" << " ";
			}
		}
		std::cout << std::endl;
	}
}

void Board::deletePreviousBoard()
{
	for (int i = 0; i < dim; i++)
	{
		delete[] boardArray[i];
		boardArray[i] = NULL;
	}
	delete[] boardArray;
	boardArray = NULL;
	delete[] tiles;
	tiles = NULL;
}

Board::~Board()
{
	for (int i = 0; i < dim; i++)
	{
		delete[] boardArray[i];
		boardArray[i] = NULL;
	}
	delete[] boardArray;
	boardArray = NULL;
	delete[] tiles;
	tiles = NULL;
}
