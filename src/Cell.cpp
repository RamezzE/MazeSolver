#include "Cell.hpp"

Cell::Cell()
{
    color = sf::Color(224, 223, 224);

    cell.setFillColor(color);
    cell.setOutlineColor(sf::Color::Yellow);

    for (int i = 0; i < 2; i++)
        line[i].setFillColor(sf::Color(21, 23, 44));

    wall[0] = false;
    wall[1] = false;

    color = sf::Color(224, 223, 224);
}

void Cell::setPosition(sf::Vector2f position)
{

    cell.setPosition(position);

    line[0].setPosition(cell.getPosition());
    line[1].setPosition(cell.getPosition());
    line[1].move(cell.getGlobalBounds().width, 0);
}

void Cell::setSize(sf::Vector2f size)
{
    cell.setSize(size);

    line[0].setSize(sf::Vector2f(cell.getGlobalBounds().width, 2));
    line[1].setSize(sf::Vector2f(2, cell.getGlobalBounds().height));
}

void Cell::setWall(int index, bool value)
{
    wall[index] = value;
}

void Cell::setVisited(bool value)
{
    visited = value;
}

void Cell::setColor(sf::Color tileColor, sf::Color wallColor) {
    cell.setFillColor(tileColor);

    for (int i = 0;i<2;i++)
        line[i].setFillColor(wallColor);

}

void Cell::resetColor()
{
    cell.setFillColor(this->color);
}

bool Cell::getWall(int index)
{
    return wall[index];
}

sf::Vector2f Cell::getPosition()
{
    return cell.getPosition();
}

sf::FloatRect Cell::getGlobalBounds() {
    return cell.getGlobalBounds();
}

bool Cell::isVisited()
{
    return visited;
}

void Cell::handleInput(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
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

void Cell::update(sf::RenderWindow *window)
{
    if (cell.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y))
        mouseOver = true;
    else
        mouseOver = false;

    // if (mouseOver)
        // cell.setOutlineThickness(5);
    // else
        // cell.setOutlineThickness(0);
}

void Cell::renderCells(sf::RenderWindow *window)
{
    window->draw(cell);
}

void Cell::renderLines(sf::RenderWindow *window)
{
    for (int i = 0; i < 2; i++)
        if (wall[i])
            window->draw(line[i]);
}
