#pragma once

#include "../../SFML/include/SFML/Graphics.hpp"
#include <iostream>
#include "Maze.hpp"

class Maze;
class Tile
{
public:
    Tile();
    Tile(Maze* maze);

    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    void setWall(int index, bool value);
    void setWallThickness(float thickness);
    void setVisited(bool value);
    void setColor(sf::Color tileColor, sf::Color wallColor);
    
    void resetColor();

    bool getWall(int index);

    sf::Vector2f getPosition();

    sf::FloatRect getGlobalBounds();

    bool isVisited();

    void handleInput(sf::Event event);

    void update(sf::RenderWindow *window);

    void renderTiles(sf::RenderWindow *window);
    void renderTiles(sf::RenderTexture *window);

    void renderLines(sf::RenderWindow *window);
    void renderLines(sf::RenderTexture *window);

private:
    Maze* maze;
    sf::RectangleShape tile;

    bool wall[2]; // top, , right
    float wallThickness;

    bool visited;
    bool destination;
    bool mouseOver;
    bool mouseIsPressed, changed;

    sf::Color color;

    sf::RectangleShape line[2];
};