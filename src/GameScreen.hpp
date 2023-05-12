#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Definitions.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "Cell.hpp"
#include "Maze.hpp"

class GameScreen : public GameState
{
public:
	GameScreen(Game *myGame);

	void handleInput();
	void update(const float dt);
	void draw();

private:
	void init();
	Game *game;

	std::vector<std::vector<Cell>> maze;
	std::vector<std::vector<sf::RectangleShape>> footprints;
	sf::RectangleShape mazeBorder;

	std::vector<std::thread> myThreads;

	sf::RectangleShape player;
};
