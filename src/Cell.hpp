#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Cell
{
public:
    Cell();

    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    void setWall(int index, bool value);
    void setVisited(bool value);

    bool getWall(int index);

    sf::Vector2f getPosition();

    bool isVisited();

    void handleInput(sf::Event event);

    void update(sf::RenderWindow *window);

    void renderCells(sf::RenderWindow *window);

    void renderLines(sf::RenderWindow *window);

private:
    sf::RectangleShape cell;

    bool wall[2]; // top, , right

    bool visited;
    bool current;
    bool mouseOver;

    sf::RectangleShape line[2];
};