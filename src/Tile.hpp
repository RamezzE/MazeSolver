#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Tile
{
public:
    Tile();

    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    void setWall(int index, bool value);
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

    void renderLines(sf::RenderWindow *window);

private:
    sf::RectangleShape tile;

    bool wall[2]; // top, , right

    bool visited;
    bool current;
    bool mouseOver;

    sf::Color color;

    sf::RectangleShape line[2];
};