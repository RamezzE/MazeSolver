#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <filesystem>

#include "ExportImageScreen.hpp"
#include "Definitions.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "Tile.hpp"
#include "Maze.hpp"
#include "Button.hpp"
#include "TextBox.hpp"
#include "Slider.hpp"

namespace fs = std::filesystem;

class ExportImageScreen; //forward declaration

class GameScreen : public GameState
{
public:
	GameScreen(Game *myGame);

	void handleInput();
	void update(const float dt);
	void draw();

	void init();

private:
	void solveMaze();
	void checkResize(sf::Event event);

	Game *game;

	Maze *maze;

	ExportImageScreen *exportImageScreen;

	std::vector<Button> myButtons;
	std::vector<TextBox> textBoxes;

	sf::Font font;

	std::vector<std::thread> myThreads;

	sf::RectangleShape background;
	sf::Texture bgImg;

	bool resizeMaze, exportMaze;

	Slider speedSlider;
	sf::Text speedLabel;
};
