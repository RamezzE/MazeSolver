#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Definitions.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "Cell.hpp"
#include "Maze.hpp"

class GameScreen : public GameState
{
public:
	GameScreen(Game* myGame);

	void handleInput();
	void update(const float dt);
	void draw();
	
private:
	void init();
	Game* game;

	std::vector<std::vector<Cell>> maze;
	sf::RectangleShape mazeBorder;

	sf::RectangleShape player;

};

