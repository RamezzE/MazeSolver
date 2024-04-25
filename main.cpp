#include <iostream>
#include <cstdlib>

#include "src/hpp/GameScreen.hpp"
#include "src/hpp/Game.hpp"

int main()
{
    Game myGame;
    myGame.changeScreen(new GameScreen(&myGame));
    myGame.gameLoop();
}