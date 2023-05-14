#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <thread>

#include "Definitions.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "Tile.hpp"
#include "Maze.hpp"
#include "Button.hpp"

class GameScreen : public GameState
{
public:
	GameScreen(Game *myGame);

	void handleInput();
	void update(const float dt);
	void draw();

private:
	void init();
	void solveMaze();
	Game *game;

	Maze* maze;

	std::vector<Button> myButtons;

	sf::Font font;

	std::vector<std::thread> myThreads;

	sf::RectangleShape background;
};
