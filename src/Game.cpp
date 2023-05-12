
#include <iostream>
#include "SFML/Graphics.hpp"

#include "Game.hpp"
#include "GameState.hpp"

Game::Game() {

    window = new sf::RenderWindow();
	width = myWidth;
	height = myHeight;
	
	window->create(sf::VideoMode(width, height), "Maze Solver", sf::Style::Titlebar | sf::Style::Close);
	window->setFramerateLimit(10);
}

void Game::changeScreen(GameState* state) {
	pushState(state);
}

void Game::previousScreen() {
	popState();
}

//let it loop back to the saved menu state
void Game::pushState(GameState* state) {
	states.push_back(state);
}

void Game::popState() {
	delete states.back();
	states.pop_back();
}


Game:: ~Game() {
	while (!states.empty()) {
		popState();
	}
}

GameState* Game::CurrentState() {
	if (states.empty()) {
		return nullptr;
	}
	else {
		return states.back();
	}
}

void Game::gameLoop() {

	sf::Clock clk;
    float dt;
	while (window->isOpen()) {

        dt = clk.restart().asSeconds();

		if (CurrentState() == nullptr) {
			continue;
		}

		window->clear();

		CurrentState()->handleInput();

		CurrentState()->update(dt);

		window->clear();

		CurrentState()->draw();

		window->display();

	}
}
