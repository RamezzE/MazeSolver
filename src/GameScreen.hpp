#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <filesystem>

#include "Definitions.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "Tile.hpp"
#include "Maze.hpp"
#include "Button.hpp"
#include "TextBox.hpp"
#include "Slider.hpp"

namespace fs = std::filesystem;

class GameScreen : public GameState
{
public:
	GameScreen(Game *myGame);

	void handleInput();
	void update(const float dt);
	void draw();

	void checkResize(sf::Event event);
	void exportMazeToPNG();

private:
	void init();
	void solveMaze();

	Game *game;

	Maze* maze;

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
