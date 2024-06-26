#pragma once

#include "../../SFML/include/SFML/Graphics.hpp"
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
#include "CheckBox.hpp"
#include "SpriteButton.hpp"

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

	sf::Font font;

	std::vector<std::thread> myThreads;

	sf::RectangleShape background;
	sf::Texture bgImg;

	std::vector<sf::RectangleShape> notesBackground;
	std::vector<sf::Text> notesText;

	bool resizeMaze, exportMaze, maximized;

	SpriteButton myGithubIcon;
	sf::Texture myGithubIconTexture;

	std::vector<SpriteButton> arrowButtons;
	std::vector<sf::Texture> arrowButtonTextures;

	std::vector<Slider*> sliders;
	std::vector<sf::Text> labels;
	std::vector<Button> myButtons;
	std::vector<TextBox> textBoxes;
	std::vector<CheckBox> checkboxes;

};
