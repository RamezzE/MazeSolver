#include <iostream>
#include <cstdlib>

#include "GameScreen.hpp"
#include "Game.hpp"
#include "GameState.hpp"

int main()
{
    Game myGame;
    myGame.changeScreen(new GameScreen(&myGame));
    myGame.gameLoop();
}