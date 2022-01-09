// sfmltest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    Game game;
    game.run();
    return 0;
}
//if (mBoard.boardArray[i][j].state == PLAYER1)
                            //{
                            //    mBoard.boardArray[i][j].state = EMPTY;
                            //    if ((i - 1 == mBoard.dim / 2) && (j == 0))
                            //    {
                            //        mBoard.boardArray[i - 1][mBoard.dim - 1].state = PLAYER1;
                            //        mTurn = PL2;
                            //        return;
                            //    }
                            //    else if (i == mBoard.dim / 2)
                            //    {
                            //        mBoard.boardArray[i][j - 1].state = PLAYER1;
                            //        mTurn = PL2;
                            //        return;
                            //    }
                            //    else if (i % 2 == 0)
                            //    {
                            //        if (j == mBoard.dim - 1)
                            //        {
                            //            mBoard.boardArray[i - 1][mBoard.dim - 1].state = PLAYER1;
                            //            //mBoard.p2Text.setPosition(mBoard.tiles[0].recTile.getPosition().x, mBoard.tiles[0].recTile.getPosition().y + (i + 1) * mBoard.tiles[0].recTile.getSize().y);
                            //            mTurn = PL2;
                            //            ////code for points etc
                            //            return;
                            //        }
                            //        else
                            //        {
                            //            mBoard.boardArray[i][j + 1].state = PLAYER1;
                            //            //mBoard.p2Text.setPosition(mBoard.p2Text.getPosition().x + mBoard.tiles[0].recTile.getSize().x, mBoard.p2Text.getPosition().y);
                            //            mTurn = PL2;
                            //            return;
                            //            //code for points etc
                            //        }
                            //    }
                            //    else if (i % 2 != 0)
                            //    {
                            //        if (j == 0)
                            //        {
                            //            mBoard.boardArray[i - 1][0].state = PLAYER1;
                            //            //mBoard.p2Text.setPosition(mBoard.tiles[0].recTile.getPosition().x, mBoard.tiles[0].recTile.getPosition().y + (i + 1) * mBoard.tiles[0].recTile.getSize().y);
                            //            mTurn = PL2;
                            //            ////code for points etc
                            //            return;
                            //        }
                            //        else
                            //        {
                            //            mBoard.boardArray[i][j - 1].state = PLAYER1;
                            //            //mBoard.p2Text.setPosition(mBoard.p2Text.getPosition().x + mBoard.tiles[0].recTile.getSize().x, mBoard.p2Text.getPosition().y);
                            //            mTurn = PL2;
                            //            return;
                            //        }
                            //    }
                            //}