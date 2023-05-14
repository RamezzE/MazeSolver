#include <iostream>
#include <cstdlib>

#include "GameScreen.hpp"
#include "Game.hpp"
#include "GameState.hpp"

template <class DataType>
int min(DataType &a, DataType &b)
{
    return a < b ? a : b;
}

int main()
{
    Game myGame;
    myGame.changeScreen(new GameScreen(&myGame));
    myGame.gameLoop();
}