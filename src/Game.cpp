#include "Game.h"
#include <iostream>
#include <fstream>

Game::Game(): mWindow(sf::VideoMode(1280, 650), "Game of Life", sf::Style::Close)
{
    font.loadFromFile("Resources/joystix monospace.ttf");

    HomeBg.loadFromFile("Resources/homebackground.png");
    HomeScreen.setTexture(HomeBg);
    homeScreenText.setFillColor(sf::Color::Black);

    homeScreenText.setFont(font);
    homeScreenText.setString("GAME OF LIFE\n\nPress :\n1 for New Game\n2 for saved game");
    sf::FloatRect textRect = homeScreenText.getLocalBounds();
    homeScreenText.setOrigin(textRect.left + textRect.width / 2.0f,
                             textRect.top + textRect.height / 2.0f);
    homeScreenText.setPosition(sf::Vector2f(1280 / 2.0f, 650 / 2.0f));

    MenuText.setFont(font);
    MenuText.setPosition(655, 5);
    MenuText.setCharacterSize(15);

    BuyMenuText.setFont(font);
    BuyMenuText.setCharacterSize(15);
    BuyMenuText.setPosition(655, 350);
    BuyMenuText.setFillColor(sf::Color::Magenta);
    buyMenuActive = false;


    statsText.setFont(font);
    statsText.setPosition(655, 570);
    statsText.setCharacterSize(15);
    statsText.setFillColor(sf::Color::Blue);

    mCurrentState = HOME;
    mTurn = Turn::PL2;
    mCurrentLevel = 1;

    levelEnd = false;

    mPlayer1.setGoldCoins(5);
    mPlayer2.setGoldCoins(5);
    mPlayer1.setSilverCoins(10);
    mPlayer2.setSilverCoins(10);
    mPlayer1.setPoints(10 + mPlayer1.getGoldCoins()*10 + mPlayer1.getSilverCoins()*5);
    mPlayer2.setPoints(10 + mPlayer2.getGoldCoins() * 10 + mPlayer2.getSilverCoins() * 5);
}

bool findItem(Items arr[], int size, Items it)
{
    bool found = false;
    for (int i = 0; i < size; i++)
    {
        if (arr[i] == it)
        {
            found = true;
            break;
        }
    }
    return found;
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed:
            handleInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handleInput(event.key.code, false);
            break;
        case sf::Event::Closed:
            mWindow.close();
            break;
        }
    }
}
void Game::update()
{
    int counter = 0;
    for (int i = 0; i < mBoard.dim; i++)
    {
        for (int j = 0; j < mBoard.dim; j++)
        {
            if (mBoard.boardArray[i][j].state == PLAYER2)
            {
                mBoard.p2Text.setPosition(j * mBoard.tiles[0].recTile.getSize().x, i * mBoard.tiles[0].recTile.getSize().y + (mBoard.tiles[0].recTile.getSize().y/3.0f));
            }
            else if (mBoard.boardArray[i][j].state == PLAYER1)
            {
                mBoard.p1Text.setPosition(j * mBoard.tiles[0].recTile.getSize().x, i * mBoard.tiles[0].recTile.getSize().y + (mBoard.tiles[0].recTile.getSize().y / 3.0f));
            }
            else if (mBoard.boardArray[i][j].state == FIRE)
            {
                mBoard.tiles[counter].sprite.setTexture(mBoard.tFire);
            }
            else if (mBoard.boardArray[i][j].state == SNAKE)
            {
                mBoard.tiles[counter].sprite.setTexture(mBoard.tSnake);
            }
            else if (mBoard.boardArray[i][j].state == GHOST)
            {
                mBoard.tiles[counter].sprite.setTexture(mBoard.tGhost);
            }
            else if (mBoard.boardArray[i][j].state == LION)
            {
                mBoard.tiles[counter].sprite.setTexture(mBoard.tLion);
            }
            else if (mBoard.boardArray[i][j].state == LOCK)
            {
                mBoard.tiles[counter].sprite.setTexture(mBoard.tLock);
            }
            else if (mBoard.boardArray[i][j].state == EMPTY)
            {
                mBoard.tiles[counter].sprite.setTexture(mBoard.dummy);
            }
            else if (mBoard.boardArray[i][j].state == GOLD)
            {
                mBoard.tiles[counter].sprite.setTexture(mBoard.tgoldCoins);
            }
            else if (mBoard.boardArray[i][j].state == SILVER)
            {
                mBoard.tiles[counter].sprite.setTexture(mBoard.tsilverCoins);
            }
            counter++;
        }
    }
    int goal = mBoard.dim / 2;
    if (mBoard.boardArray[goal][goal].state == PLAYER1)
    {
        levelEnd = true;
        buyMenuActive = false;
        if (mPlayer1.getPoints() < mPlayer2.getPoints())
        {
            menu = "Too bad Player 1, this is a draw. Game Over.";
        }
        else
        {
            menu = "PLAYER1 is the winner!!!";
        }
    }
    else if (mBoard.boardArray[goal][goal].state == PLAYER2)
    {
        levelEnd = true;
        if (mPlayer2.getPoints() < mPlayer1.getPoints())
        {
            menu = "Too bad Player 2, this is a draw. Game Over.";
        }
        else
        {
            menu = "PLAYER2 is the winner!!!";
        }
    }
    if (levelEnd)
    {
        if (mCurrentLevel < 4)
            menu += "\nPress Enter to go to next level";
        else
            menu += "\n\n\nIt was fun playing. You can exit the game now\nbut dont forget to come back later for fun.";
    }
    stats = "Player 1 Gold Coins : " + std::to_string(mPlayer1.getGoldCoins()) +
        "  Player 2 Gold Coins : " + std::to_string(mPlayer2.getGoldCoins()) +
        "\nPlayer 1 Silv Coins : " + std::to_string(mPlayer1.getSilverCoins()) +
        "  Player 2 Silv Coins : " + std::to_string(mPlayer2.getSilverCoins()) +
        "\nPlayer 1 points : " + std::to_string(mPlayer1.getPoints()) +
        "     Player 2 points : " + std::to_string(mPlayer2.getPoints());
    statsText.setString(stats);
    Items* temp;
    if (!levelEnd)
    {
        if (mTurn == PL1)
        {
            menu = "Player 1, you have following items in your box";
            temp = mPlayer1.getInventory();
            for (int i = 0; i < mPlayer1.getnItems(); i++)
            {
                if (temp[i] == iKEY)
                {
                    menu += "\nKEY";
                }
                else if (temp[i] == iSHIELD)
                {
                    menu += "\nSHIELD";
                }
                else if (temp[i] == iWATER)
                {
                    menu += "\nWATER";
                }
                else if (temp[i] == iSWORD)
                {
                    menu += "\nSWORD";
                }
            }
        }
        else if (mTurn == PL2)
        {
            menu = "Player 2, you have following items in your box";
            temp = mPlayer2.getInventory();
            for (int i = 0; i < mPlayer2.getnItems(); i++)
            {
                if (temp[i] == iKEY)
                {
                    menu += "\nKEY";
                }
                else if (temp[i] == iSHIELD)
                {
                    menu += "\nSHIELD";
                }
                else if (temp[i] == iWATER)
                {
                    menu += "\nWATER";
                }
                else if (temp[i] == iSWORD)
                {
                    menu += "\nSWORD";
                }
            }
        }
        menu += "\n\nPress\n\nSPACEBAR to move\nR to use SWORD\nV to use Water\nE to use shield\nY to use Key\nM To open buy menu\nQ to save current game state";
    }
    MenuText.setString(menu);
}

void Game::handleInput(sf::Keyboard::Key key, bool pressed)
{
    //TODO: Add to it later
    switch (key)
    {
    case sf::Keyboard::Return:
        if (mCurrentState == State::ACTION && levelEnd && mCurrentLevel < 4)
        {
            levelEnd = false;
            mCurrentLevel++;
            mPlayer1.resetVariables();
            mPlayer2.resetVariables();
            mBoard.deletePreviousBoard();
            mBoard.initializeBoard(mBoard.dim + 2);
        }
        break;
    case sf::Keyboard::Num1:
        if (mCurrentState == State::HOME)
        {
            mCurrentState = State::ACTION;
        }
        break;
    case sf::Keyboard::Num2:
        if (mCurrentState == HOME)
        {
            std::ifstream f;
            f.open("saveddata.txt");
            int dim;
            f >> dim;
            mBoard.deletePreviousBoard();
            mBoard.initializeBoard(dim);
            loadGame();
            mCurrentState = ACTION;
            mTurn = PL1;
        }
        break;

    case sf::Keyboard::Space:
        if(mCurrentState == State::ACTION)
        {
            if (pressed && !levelEnd)
            {
                if (mTurn == PL2)
                {
                    if (mPlayer2.getMovesToSkip() == 0 && !mPlayer2.getLockedStatus())
                    {
                        for (int i = 0; i < mBoard.dim; i++)
                        {
                            for (int j = 0; j < mBoard.dim; j++)
                            {
                                if (mBoard.boardArray[i][j].state == PLAYER2)
                                {
                                    mBoard.boardArray[i][j].state = EMPTY;
                                    if (j == mBoard.dim - 1)
                                    {
                                        if (mBoard.boardArray[i + 1][0].state == GOLD)
                                        {
                                            mPlayer2.setGoldCoins(mPlayer2.getGoldCoins() + 1);
                                            mPlayer2.setPoints(mPlayer2.getPoints() + 10);
                                        }
                                        else if (mBoard.boardArray[i + 1][0].state == SILVER)
                                        {
                                            mPlayer2.setSilverCoins(mPlayer2.getSilverCoins() + 1);
                                            mPlayer2.setPoints(mPlayer2.getPoints() + 5);
                                        }
                                        mBoard.boardArray[i + 1][0].state = PLAYER2;
                                        mTurn = PL1;
                                        return;
                                    }
                                    else
                                    {
                                        if (mBoard.boardArray[i][j + 1].state == GOLD)
                                        {
                                            mPlayer2.setGoldCoins(mPlayer2.getGoldCoins() + 1);
                                            mPlayer2.setPoints(mPlayer2.getPoints() + 10);
                                        }
                                        else if (mBoard.boardArray[i][j + 1].state == SILVER)
                                        {
                                            mPlayer2.setSilverCoins(mPlayer2.getSilverCoins() + 1);
                                            mPlayer2.setPoints(mPlayer2.getPoints() + 5);
                                        }
                                        mBoard.boardArray[i][j + 1].state = PLAYER2;
                                        //mBoard.p2Text.setPosition(mBoard.p2Text.getPosition().x + mBoard.tiles[0].recTile.getSize().x, mBoard.p2Text.getPosition().y);
                                        mTurn = PL1;
                                        return;
                                        //code for points etc
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if (mPlayer2.getMovesToSkip() > 0)
                        {
                            mPlayer2.decrementMovesToSkip();
                        }
                        mTurn = PL1;
                        return;
                    }
                }
                else if (mTurn == PL1)
                {
                    if (mPlayer1.getMovesToSkip() == 0 && !mPlayer1.getLockedStatus())
                    {
                        for (int i = 0; i < mBoard.dim; i++)
                        {
                            for (int j = 0; j < mBoard.dim; j++)
                            {
                                if (mBoard.boardArray[i][j].state == PLAYER1)
                                {
                                    mBoard.boardArray[i][j].state = EMPTY;
                                    if ((i - 1 == mBoard.dim / 2) && (j == mBoard.dim - 1))
                                    {
                                        if (mBoard.boardArray[i - 1][mBoard.dim - 1].state == GOLD)
                                        {
                                            mPlayer1.setGoldCoins(mPlayer1.getGoldCoins() + 1);
                                            mPlayer1.setPoints(mPlayer2.getPoints() + 10);
                                        }
                                        else if (mBoard.boardArray[i - 1][mBoard.dim - 1].state == SILVER)
                                        {
                                            mPlayer1.setSilverCoins(mPlayer1.getSilverCoins() + 1);
                                            mPlayer1.setPoints(mPlayer2.getPoints() + 5);

                                        }
                                        mBoard.boardArray[i - 1][mBoard.dim - 1].state = PLAYER1;
                                        mTurn = PL2;
                                        return;
                                    }
                                    else if (i == mBoard.dim / 2)
                                    {
                                        if (mBoard.boardArray[i][j - 1].state == GOLD)
                                        {
                                            mPlayer1.setGoldCoins(mPlayer1.getGoldCoins() + 1);
                                            mPlayer1.setPoints(mPlayer2.getPoints() + 10);

                                        }
                                        else if (mBoard.boardArray[i][j - 1].state == SILVER)
                                        {
                                            mPlayer1.setSilverCoins(mPlayer1.getSilverCoins() + 1);
                                            mPlayer1.setPoints(mPlayer2.getPoints() + 5);

                                        }
                                        mBoard.boardArray[i][j - 1].state = PLAYER1;
                                        mTurn = PL2;
                                        return;
                                    }
                                    else if (j == mBoard.dim - 1)
                                    {
                                        if (mBoard.boardArray[i - 1][0].state == GOLD)
                                        {
                                            mPlayer1.setGoldCoins(mPlayer1.getGoldCoins() + 1);
                                            mPlayer1.setPoints(mPlayer2.getPoints() + 5);

                                        }
                                        else if (mBoard.boardArray[i - 1][0].state == SILVER)
                                        {
                                            mPlayer1.setSilverCoins(mPlayer1.getSilverCoins() + 1);
                                            mPlayer1.setPoints(mPlayer2.getPoints() + 5);

                                        }
                                        mBoard.boardArray[i - 1][0].state = PLAYER1;
                                       
                                        mTurn = PL2;
       
                                        return;
                                    }
                                    else
                                    {
                                        if (mBoard.boardArray[i][j + 1].state == GOLD)
                                        {
                                            mPlayer1.setGoldCoins(mPlayer1.getGoldCoins() + 1);
                                            mPlayer1.setPoints(mPlayer2.getPoints() + 10);

                                        }
                                        else if (mBoard.boardArray[i][j + 1].state == SILVER)
                                        {
                                            mPlayer1.setSilverCoins(mPlayer1.getSilverCoins() + 1);
                                            mPlayer1.setPoints(mPlayer2.getPoints() + 5);

                                        }
                                        mBoard.boardArray[i][j + 1].state = PLAYER1;
                                        
                                        mTurn = PL2;
                                        return;
                                        
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        if (mPlayer1.getMovesToSkip() > 0)
                        {
                            mPlayer1.decrementMovesToSkip();
                        }
                        mTurn = PL2;
                        return;
                    }
                }
                break;
            }
        }
    case sf::Keyboard::M:
        if (mCurrentState == ACTION && pressed && !levelEnd)
        {
            buyMenuActive = true;
            std::string buyText = "\t\t!!!BUY MENU!!!\nPress\nS to buy SWORD\nW to buy WATER\nK to buy KEY\nD to buy Shield\nF to buy FIRE\nG to buy ghost\nN to buy SNAKE\nL to buy LION\nO to buy Lock";
            BuyMenuText.setString(buyText);
        }
        break;
    case sf::Keyboard::F:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getPoints() >= 50)
                {
                    mPlayer1.setPoints(mPlayer1.getPoints() - 50);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER2)
                            {
                                if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i + 1][0].state = FIRE;
                                    buyMenuActive = false;
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = FIRE;
                                    buyMenuActive = false;
                                }
                                mPlayer2.setMovesToSkip(2);
                            }
                        }
                    }
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getPoints() >= 50)
                {
                    mPlayer2.setPoints(mPlayer2.getPoints() - 50);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER1)
                            {
                                if ((i - 1 == mBoard.dim / 2) && (j == mBoard.dim - 1))
                                {
                                    mBoard.boardArray[i - 1][mBoard.dim - 1].state = FIRE;
                                    buyMenuActive = false;
                                }
                                else if (i == mBoard.dim / 2)
                                {
                                    mBoard.boardArray[i][j - 1].state = FIRE;
                                    buyMenuActive = false;
                                }
                                else if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i - 1][0].state = FIRE;
                                    buyMenuActive = false;
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = FIRE;
                                    buyMenuActive = false;
                                }
                                mPlayer1.setMovesToSkip(2);
                            }
                        }
                    }
                }
            }
        }
        break;
    case sf::Keyboard::G:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getPoints() >= 20)
                {
                    mPlayer1.setPoints(mPlayer1.getPoints() - 20);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER2)
                            {
                                if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i + 1][0].state = GHOST;
                                    buyMenuActive = false;
                                    
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = GHOST;
                                    buyMenuActive = false;
                                }
                                mPlayer2.setMovesToSkip(1);
                            }
                        }
                    }
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getPoints() >= 20)
                {
                    mPlayer2.setPoints(mPlayer2.getPoints() - 20);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER1)
                            {
                                if ((i - 1 == mBoard.dim / 2) && (j == mBoard.dim - 1))
                                {
                                    mBoard.boardArray[i - 1][mBoard.dim - 1].state = GHOST;
                                    buyMenuActive = false;
                                }
                                else if (i == mBoard.dim / 2)
                                {
                                    mBoard.boardArray[i][j - 1].state = GHOST;
                                    buyMenuActive = false;
                                }
                                else if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i - 1][0].state = GHOST;
                                    buyMenuActive = false;
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = GHOST;
                                    buyMenuActive = false;
                                }
                                mPlayer1.setMovesToSkip(1);
                            }
                        }
                    }
                }
            }
        }
        break;
    case sf::Keyboard::N:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getPoints() >= 30)
                {
                    mPlayer1.setPoints(mPlayer1.getPoints() - 30);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER2)
                            {
                                if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i + 1][0].state = SNAKE;
                                    mPlayer2.setMovesToSkip(3);
                                    buyMenuActive = false;
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = SNAKE;
                                    mPlayer2.setMovesToSkip(3);
                                    buyMenuActive = false;
                                }
                            }
                        }
                    }
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getPoints() >= 30)
                {
                    mPlayer2.setPoints(mPlayer2.getPoints() - 30);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER1)
                            {
                                if ((i - 1 == mBoard.dim / 2) && (j == mBoard.dim - 1))
                                {
                                    mBoard.boardArray[i - 1][mBoard.dim - 1].state = SNAKE;
                                    buyMenuActive = false;
                                }
                                else if (i == mBoard.dim / 2)
                                {
                                    mBoard.boardArray[i][j - 1].state = SNAKE;
                                    buyMenuActive = false;
                                }
                                else if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i - 1][0].state = SNAKE;
                                    buyMenuActive = false;
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = SNAKE;
                                    buyMenuActive = false;
                                }
                                mPlayer1.setMovesToSkip(3);
                            }
                        }
                    }
                }
            }
        }
        break;
    case sf::Keyboard::L:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getGoldCoins() >= 5)
                {
                    mPlayer1.setGoldCoins(mPlayer1.getGoldCoins() - 5);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER2)
                            {
                                if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i + 1][0].state = LION;
                                    buyMenuActive = false;
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = LION;
                                    buyMenuActive = false;
                                }
                                mPlayer2.setMovesToSkip(4);
                            }
                        }
                    }
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getGoldCoins() >= 5)
                {
                    mPlayer2.setGoldCoins(mPlayer2.getGoldCoins() - 5);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER1)
                            {
                                if ((i - 1 == mBoard.dim / 2) && (j == mBoard.dim - 1))
                                {
                                    mBoard.boardArray[i - 1][mBoard.dim - 1].state = LION;
                                    buyMenuActive = false;
                                }
                                else if (i == mBoard.dim / 2)
                                {
                                    mBoard.boardArray[i][j - 1].state = LION;
                                    buyMenuActive = false;
                                }
                                else if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i - 1][0].state = LION;
                                    buyMenuActive = false;
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = LION;
                                    buyMenuActive = false;
                                }
                                mPlayer1.setMovesToSkip(4);
                            }
                        }
                    }
                }
            }
        }
        break;
    case sf::Keyboard::O:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getSilverCoins() >= 12)
                {
                    mPlayer1.setSilverCoins(mPlayer1.getSilverCoins() - 12);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER2)
                            {
                                if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i + 1][0].state = LOCK;
                                    buyMenuActive = false;
                                    mPlayer2.setLockedStatus(true);
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = LOCK;
                                    buyMenuActive = false;
                                    mPlayer2.setLockedStatus(true);
                                }
                            }
                        }
                    }
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getSilverCoins() >= 12)
                {
                    mPlayer2.setSilverCoins(mPlayer2.getSilverCoins() - 12);
                    for (int i = 0; i < mBoard.dim; i++)
                    {
                        for (int j = 0; j < mBoard.dim; j++)
                        {
                            if (mBoard.boardArray[i][j].state == PLAYER1)
                            {
                                if ((i - 1 == mBoard.dim / 2) && (j == mBoard.dim - 1))
                                {
                                    mBoard.boardArray[i - 1][mBoard.dim - 1].state = LOCK;
                                    buyMenuActive = false;
                                    //mPlayer1.setLockedStatus(true);
                                }
                                else if (i == mBoard.dim / 2)
                                {
                                    mBoard.boardArray[i][j - 1].state = LOCK;
                                    buyMenuActive = false;
                                    //mPlayer1.setLockedStatus(true);
                                }
                                else if (j == mBoard.dim - 1)
                                {
                                    mBoard.boardArray[i - 1][0].state = LOCK;
                                    buyMenuActive = false;
                                }
                                else
                                {
                                    mBoard.boardArray[i][j + 1].state = LOCK;
                                    buyMenuActive = false;
                                }
                                mPlayer1.setLockedStatus(true);
                            }
                        }
                    }
                }
            }
        }
        break;
    case sf::Keyboard::S:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getPoints() >= 40 && mPlayer1.getSwordUse() < 2)
                {
                    mPlayer1.setPoints(mPlayer1.getPoints() - 40);
                    mPlayer1.addToInventory(iSWORD);
                    mPlayer1.incrementSwordUse();
                    buyMenuActive = false;
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getPoints() >= 40 && mPlayer2.getSwordUse() < 2)
                {
                    mPlayer2.setPoints(mPlayer2.getPoints() - 40);
                    mPlayer2.addToInventory(iSWORD);
                    mPlayer2.incrementSwordUse();
                    buyMenuActive = false;
                }
            }
        }
        break;
    case sf::Keyboard::W:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getPoints() >= 50 && mPlayer1.getWaterUse() < 1)
                {
                    mPlayer1.setPoints(mPlayer1.getPoints() - 50);
                    mPlayer1.addToInventory(iWATER);
                    mPlayer1.incrementWaterUse();
                    buyMenuActive = false;
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getPoints() >= 50 && mPlayer2.getWaterUse() < 1)
                {
                    mPlayer2.setPoints(mPlayer2.getPoints() - 50);
                    mPlayer2.addToInventory(iWATER);
                    mPlayer2.incrementWaterUse();
                    buyMenuActive = false;
                }
            }
        }
        break;
        case sf::Keyboard::K:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getPoints() >= 70)
                {
                    mPlayer1.setPoints(mPlayer1.getPoints() - 70);
                    mPlayer1.addToInventory(iKEY);
                    buyMenuActive = false;
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getPoints() >= 70)
                {
                    mPlayer2.setPoints(mPlayer2.getPoints() - 70);
                    mPlayer2.addToInventory(iKEY);
                    buyMenuActive = false;
                }
            }
        }
        break;
        case sf::Keyboard::D:
        if (buyMenuActive)
        {
            if (mTurn == PL1)
            {
                if (mPlayer1.getPoints() >= 30 && mPlayer1.getShieldUse() < 1)
                {
                    mPlayer1.setPoints(mPlayer1.getPoints() - 30);
                    mPlayer1.addToInventory(iSHIELD);
                    mPlayer1.incrementShieldUse();
                    buyMenuActive = false;
                }
            }
            else if (mTurn == PL2)
            {
                if (mPlayer2.getPoints() >= 30 && mPlayer2.getShieldUse())
                {
                    mPlayer2.setPoints(mPlayer2.getPoints() - 30);
                    mPlayer2.addToInventory(iSHIELD);
                    mPlayer2.incrementShieldUse();
                    buyMenuActive = false;
                }
            }
        }
        break;
        case sf::Keyboard::Y:
        {
            if (mCurrentState == ACTION)
            {
                if (mTurn == PL1)
                {
                    Items* temp = mPlayer1.getInventory();
                    if (mPlayer1.getLockedStatus() == true && findItem(temp, mPlayer1.getnItems(),iKEY))
                    {
                        mPlayer1.setLockedStatus(false);
                        mPlayer1.removeFromInventory(iKEY);
                    }
                }
                else if (mTurn == PL2)
                {
                    Items* temp = mPlayer2.getInventory();
                    if (mPlayer2.getLockedStatus() && findItem(temp, mPlayer2.getnItems(), iKEY))
                    {
                        mPlayer2.setLockedStatus(false);
                        mPlayer2.removeFromInventory(iKEY);
                    }
                }
            }
            break;
        }
        case sf::Keyboard::R:
            if (mCurrentState == ACTION)
            {
                if (mTurn == PL1)
                {
                    Items* temp = mPlayer1.getInventory();
                    if (findItem(temp, mPlayer1.getnItems(), iSWORD))
                    {
                        int mid = mBoard.dim / 2;
                        for (int i = mid; i < mBoard.dim; i++)
                        {
                            for (int j = 0; j < mBoard.dim; j++)
                            {
                                if (mBoard.boardArray[i][j].state == LION || mBoard.boardArray[i][j].state == SNAKE)
                                {
                                    mBoard.boardArray[i][j].state = EMPTY;
                                    mPlayer1.removeFromInventory(iSWORD);
                                    mPlayer1.setMovesToSkip(0);
                                    return;
                                }
                            }
                        }
                    }
                }
                else if(mTurn == PL2)
                {
                    Items* temp = mPlayer2.getInventory();
                    if (findItem(temp, mPlayer2.getnItems(), iSWORD))
                    {
                        for (int i = 0; i < mBoard.dim; i++)
                        {
                            for (int j = 0; j < mBoard.dim; j++)
                            {
                                if (mBoard.boardArray[i][j].state == LION || mBoard.boardArray[i][j].state == SNAKE)
                                {
                                    mBoard.boardArray[i][j].state = EMPTY;
                                    mPlayer2.removeFromInventory(iSWORD);
                                    mPlayer2.setMovesToSkip(0);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case sf::Keyboard::V:
            if (mCurrentState == ACTION)
            {
                if (mTurn == PL1)
                {
                    Items* temp = mPlayer1.getInventory();
                    if (findItem(temp, mPlayer1.getnItems(), iWATER))
                    {
                        int mid = mBoard.dim / 2;
                        for (int i = mid; i < mBoard.dim; i++)
                        {
                            for (int j = 0; j < mBoard.dim; j++)
                            {
                                if (mBoard.boardArray[i][j].state == FIRE)
                                {
                                    mBoard.boardArray[i][j].state = EMPTY;
                                    mPlayer1.removeFromInventory(iWATER);
                                    mPlayer1.setMovesToSkip(0);
                                    return;
                                }
                            }
                        }
                    }
                }
                else if (mTurn == PL2)
                {
                    Items* temp = mPlayer2.getInventory();
                    if (findItem(temp, mPlayer2.getnItems(), iWATER))
                    {
                        for (int i = 0; i < mBoard.dim; i++)
                        {
                            for (int j = 0; j < mBoard.dim; j++)
                            {
                                if (mBoard.boardArray[i][j].state == FIRE)
                                {
                                    mBoard.boardArray[i][j].state = EMPTY;
                                    mPlayer2.removeFromInventory(iWATER);
                                    mPlayer2.setMovesToSkip(0);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case sf::Keyboard::E:
            if (mCurrentState == ACTION)
            {
                if (mTurn == PL1)
                {
                    Items* temp = mPlayer1.getInventory();
                    if (findItem(temp, mPlayer1.getnItems(), iSHIELD))
                    {
                        int mid = mBoard.dim / 2;
                        for (int i = mid; i < mBoard.dim; i++)
                        {
                            for (int j = 0; j < mBoard.dim; j++)
                            {
                                if (mBoard.boardArray[i][j].state == GHOST)
                                {
                                    mBoard.boardArray[i][j].state = EMPTY;
                                    mPlayer1.removeFromInventory(iSHIELD);
                                    mPlayer1.setMovesToSkip(0);
                                    return;
                                }
                            }
                        }
                    }
                }
                else if (mTurn == PL2)
                {
                    Items* temp = mPlayer2.getInventory();
                    if (findItem(temp, mPlayer2.getnItems(), iSHIELD))
                    {
                        for (int i = 0; i < mBoard.dim; i++)
                        {
                            for (int j = 0; j < mBoard.dim; j++)
                            {
                                if (mBoard.boardArray[i][j].state == GHOST)
                                {
                                    mBoard.boardArray[i][j].state = EMPTY;
                                    mPlayer2.removeFromInventory(iSHIELD);
                                    mPlayer2.setMovesToSkip(0);
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            break;
        case sf::Keyboard::Q:
        {
            saveGameState();
            std::cout << "Game data saved" << std::endl;
            break;
        }
    }
}

void Game::render()
{
    mWindow.clear();
    switch (mCurrentState)
    {
    case State::HOME:
        mWindow.draw(HomeScreen);
        mWindow.draw(homeScreenText);
        break;
    case State::ACTION:
        for (int i = 0; i < mBoard.dim*mBoard.dim; i++)
        {
            mWindow.draw(mBoard.tiles[i].recTile);
            mWindow.draw(mBoard.tiles[i].text);
            mWindow.draw(mBoard.tiles[i].sprite);
        }
        mWindow.draw(mBoard.p2Text);
        mWindow.draw(mBoard.p1Text);
        mWindow.draw(MenuText);
        mWindow.draw(statsText);
        if(buyMenuActive)
            mWindow.draw(BuyMenuText);
        break;
    }
    mWindow.display();
}

void Game::run()
{
    while (mWindow.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::saveGameState()
{
    std::ofstream file;
    file.open("saveddata.txt");
    file << mBoard.dim << std::endl;
    file << mCurrentLevel << std::endl;
    for (int i = 0; i < mBoard.dim; i++)
    {
        for (int j = 0; j < mBoard.dim; j++)
        {
            if (mBoard.boardArray[i][j].state == EMPTY)
            {
                file << 0 << " ";
            }
            else if (mBoard.boardArray[i][j].state == PLAYER1)
            {
                file << 1 << " ";
            }
            else if (mBoard.boardArray[i][j].state == PLAYER2)
            {
                file << 2 << " ";
            }
            else if (mBoard.boardArray[i][j].state == GOLD)
            {
                file << 3 << " ";
            }
            else if (mBoard.boardArray[i][j].state == SILVER)
            {
                file << 4 << " ";
            }
            else if (mBoard.boardArray[i][j].state == FIRE)
            {
                file << 5 << " ";
            }
            else if (mBoard.boardArray[i][j].state == SNAKE)
            {
                file << 6 << " ";
            }
            else if (mBoard.boardArray[i][j].state == GHOST)
            {
                file << 7 << " ";
            }
            else if (mBoard.boardArray[i][j].state == LION)
            {
                file << 8 << " ";
            }
            else if (mBoard.boardArray[i][j].state == LOCK)
            {
                file << 9 << " ";
            }
            else if (mBoard.boardArray[i][j].state == GOAL)
            {
                file << 10 << " ";
            }
        }
    }
    file << std::endl;
    Items* temp = mPlayer1.getInventory();
    file << mPlayer1.getnItems() << std::endl;
    file << mPlayer1.getPoints() << std::endl;
    file << mPlayer1.getGoldCoins() << std::endl;
    file << mPlayer1.getSilverCoins() << std::endl;
    file << mPlayer1.getMovesToSkip() << std::endl;
    file << mPlayer1.getLockedStatus() << std::endl;
    file << mPlayer1.getSwordUse() << std::endl;
    file << mPlayer1.getWaterUse() << std::endl;
    file << mPlayer1.getShieldUse() << std::endl;
    for (int i = 0; i < mPlayer1.getnItems(); i++)
    {
        file << temp[i] << " ";
    }
    file << std::endl;
    temp = mPlayer2.getInventory();
    file << mPlayer2.getnItems() << std::endl;
    file << mPlayer2.getPoints() << std::endl;
    file << mPlayer2.getGoldCoins() << std::endl;
    file << mPlayer2.getSilverCoins() << std::endl;
    file << mPlayer2.getMovesToSkip() << std::endl;
    file << mPlayer2.getLockedStatus() << std::endl;
    file << mPlayer2.getSwordUse() << std::endl;
    file << mPlayer2.getWaterUse() << std::endl;
    file << mPlayer2.getShieldUse() << std::endl;
    for (int i = 0; i < mPlayer2.getnItems(); i++)
    {
        file << temp[i] << " ";
    }
    file << std::endl;
    file << buyMenuActive << std::endl;
    file << levelEnd << std::endl;
    file.close();
}

void Game::loadGame()
{
    std::ifstream file;
    file.open("saveddata.txt");
    file >> mBoard.dim;
    file >> mCurrentLevel;
    int c = 0;
    for (int i = 0; i < mBoard.dim; i++)
    {
        for (int j = 0; j < mBoard.dim; j++)
        {
            int f;
            file >> f;
            if (f == 0)
            {
                mBoard.boardArray[i][j].state = EMPTY;
            }
            else if(f==1)
            {
                mBoard.boardArray[i][j].state = PLAYER1;
            }
            else if (f == 2)
            {
                mBoard.boardArray[i][j].state = PLAYER2;
            }
            else if (f == 3)
            {
                mBoard.boardArray[i][j].state = GOLD;
            }
            else if (f == 4)
            {
                mBoard.boardArray[i][j].state = SILVER;
            }
            else if (f == 5)
            {
                mBoard.boardArray[i][j].state = FIRE;
            }
            else if (f == 6)
            {
                mBoard.boardArray[i][j].state = SNAKE;
            }
            else if (f == 7)
            {
                mBoard.boardArray[i][j].state = GHOST;
            }
            else if (f == 8)
            {
                mBoard.boardArray[i][j].state = LION;
            }
            else if (f == 9)
            {
                mBoard.boardArray[i][j].state = LOCK;
            }
            else if (f == 10)
            {
                mBoard.boardArray[i][j].state = GOAL;
            }
        }
    }
    int nitems, points, gold, silver, moves, locked, sworduse, wateruse, shielduse;
    file >> nitems;
    file >> points;
    file >> gold;
    file >> silver;
    file >> moves;
    file >> locked;
    file >> sworduse;
    file >> wateruse;
    file >> shielduse;
    mPlayer1.setGoldCoins(gold);
    mPlayer1.setSilverCoins(silver);
    mPlayer1.setMovesToSkip(moves);
    mPlayer1.setLockedStatus(locked);
    mPlayer1.setSwordUse(sworduse);
    mPlayer1.setWaterUse(wateruse);
    mPlayer1.setShieldUse(shielduse);
    for (int i = 0; i < nitems; i++)
    {
        int f;
        file >> f;
        if (f == 0)
            mPlayer1.addToInventory(iSWORD);
        else if (f == 1)
            mPlayer1.addToInventory(iWATER);
        else if (f == 2)
            mPlayer1.addToInventory(iSHIELD);
        else if (f == 3)
            mPlayer1.addToInventory(iKEY);
    }
    mPlayer1.setnItems(nitems);
    file >> nitems;
    file >> points;
    file >> gold;
    file >> silver;
    file >> moves;
    file >> locked;
    file >> sworduse;
    file >> wateruse;
    file >> shielduse;
    mPlayer2.setGoldCoins(gold);
    mPlayer2.setSilverCoins(silver);
    mPlayer2.setMovesToSkip(moves);
    mPlayer2.setLockedStatus(locked);
    mPlayer2.setSwordUse(sworduse);
    mPlayer2.setWaterUse(wateruse);
    mPlayer2.setShieldUse(shielduse);
    for (int i = 0; i < nitems; i++)
    {
        int f;
        file >> f;
        if (f == 0)
            mPlayer2.addToInventory(iSWORD);
        else if (f == 1)
            mPlayer2.addToInventory(iWATER);
        else if (f == 2)
            mPlayer2.addToInventory(iSHIELD);
        else if (f == 3)
            mPlayer2.addToInventory(iKEY);
    }
    mPlayer2.setnItems(nitems);
    file >> buyMenuActive;
    file >> levelEnd;
    file.close();
}