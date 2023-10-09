#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

#include "GameState.hpp"
#include "Definitions.hpp"

typedef unsigned short ushort;

class Game {
public:
	Game();
	~Game();

	void changeScreen(GameState* state);
	void previousScreen();

	GameState* getPreviousScreen();
	
	void gameLoop();

	sf::RenderWindow* window;
    ushort width, height;

private:
	std::vector<GameState*> states;
	GameState* CurrentState();
	
	void pushState(GameState* state);
	void popState();

	sf::Image icon;

};