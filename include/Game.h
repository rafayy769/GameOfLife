#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Board.h"
#include "Player.h"

enum State
{
	HOME,
	ACTION
};

enum Turn
{
	PL1,
	PL2
};

class Game
{
	sf::RenderWindow mWindow;
	sf::Font font;
	sf::Texture HomeBg;
	sf::Sprite HomeScreen;

	sf::Text homeScreenText;
	sf::Text MenuText;
	sf::Text statsText;
	sf::Text BuyMenuText;

	bool buyMenuActive;
	bool levelEnd;

	unsigned short mCurrentLevel;

	std::string stats;
	std::string menu;
	Board mBoard;

	Player mPlayer1;
	Player mPlayer2;

	Turn mTurn;

	State mCurrentState;

	void processEvents();													  //Member function that'll handle all the EVENTS happening (Events as in the windows events) 
	void update();															  //Member function that'll update the value of different variables as the game progresses
	void render();															  //Member function that'll be rendering all the screens and objects, param is DelayedText object, to display at the homescreen
	void handleInput(sf::Keyboard::Key key, bool pressed);					  //Member function that'll recieve player's input and takes actions accordingly
	
	void saveGameState();
	void loadGame();
public:
	Game();
	void run();
};

