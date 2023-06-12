#include "Tile.hpp"

Tile::Tile()
{
    color = sf::Color(224, 223, 224);

    tile.setFillColor(color);

    for (int i = 0; i < 2; i++)
        line[i].setFillColor(sf::Color(21, 23, 44));

    wall[0] = true;
    wall[1] = true;
    wallThickness = 2;

    color = sf::Color(224, 223, 224);

    destination = false;
}

void Tile::setPosition(sf::Vector2f position)
{

    tile.setPosition(position);

    line[0].setPosition(tile.getPosition());
    line[1].setPosition(tile.getPosition());
    line[1].move(tile.getGlobalBounds().width, 0);
}

void Tile::setSize(sf::Vector2f size)
{
    tile.setSize(size);

    line[0].setSize(sf::Vector2f(tile.getGlobalBounds().width, wallThickness));
    line[1].setSize(sf::Vector2f(wallThickness, tile.getGlobalBounds().height));
}

void Tile::setWallThickness(float thickness)
{
    wallThickness = thickness;
    setSize(tile.getSize());
}

void Tile::setWall(int index, bool value)
{
    wall[index] = value;
}

void Tile::setVisited(bool value)
{
    visited = value;
}

void Tile::setColor(sf::Color tileColor, sf::Color wallColor)
{
    tile.setFillColor(tileColor);

    for (int i = 0; i < 2; i++)
        line[i].setFillColor(wallColor);
}

void Tile::resetColor()
{
    tile.setFillColor(this->color);
    line[0].setFillColor(sf::Color(21, 23, 44));
    line[1].setFillColor(sf::Color(21, 23, 44));
}

bool Tile::getWall(int index)
{
    return wall[index];
}

sf::Vector2f Tile::getPosition()
{
    return tile.getPosition();
}

sf::FloatRect Tile::getGlobalBounds()
{
    return tile.getGlobalBounds();
}

bool Tile::isVisited()
{
    return visited;
}

void Tile::handleInput(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (mouseOver)
                wall[0] = !wall[0];
        }
        else if (event.mouseButton.button == sf::Mouse::Right)
        {
            if (mouseOver)
                wall[1] = !wall[1];
        }
    }
}

void Tile::update(sf::RenderWindow *window)
{
    if (tile.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
        mouseOver = true;
    else
        mouseOver = false;
}

void Tile::renderTiles(sf::RenderWindow *window)
{
    window->draw(tile);
}

void Tile::renderTiles(sf::RenderTexture *window)
{
    window->draw(tile);
}

void Tile::renderLines(sf::RenderWindow *window)
{
    for (int i = 0; i < 2; i++)
        if (wall[i])
            window->draw(line[i]);
}

void Tile::renderLines(sf::RenderTexture *window)
{
    for (int i = 0; i < 2; i++)
        if (wall[i])
            window->draw(line[i]);
}