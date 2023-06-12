#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>

#include "Definitions.hpp"
#include "Game.hpp"
#include "GameState.hpp"
#include "Maze.hpp"
#include "GameScreen.hpp"
#include "Button.hpp"


namespace fs = std::filesystem;

class GameScreen; //forward declaration

class ExportImageScreen : public GameState
{
public:
    ExportImageScreen(Game *myGame, Maze *maze);

    void handleInput();
    void update(const float dt);
    void draw();

    void exportMazeToPNG();
    void init();

private:
    void previewMaze();
    void checkResize(sf::Event event);

    Game *game;
    Maze *maze;

    sf::Font font;

    sf::RectangleShape background;
    sf::Texture bgImg;

    std::vector<Button> myButtons;

    sf::RenderTexture mazeRenderTexture;
    sf::Sprite mazePreview;

    sf::Image mazeImage;

    sf::Text imagePreviewLabel;

    sf::Vector2f mazePreviewSize;
};
