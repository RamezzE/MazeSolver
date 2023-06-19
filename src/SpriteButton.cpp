#include "SpriteButton.hpp"

#include <iostream>
SpriteButton::SpriteButton()
{
    init();
}

SpriteButton::SpriteButton(sf::Texture &texture)
{
    sprite.setTexture(texture);
    init();
}

void SpriteButton::init()
{
    mouseOver = pressed = doAction = false;
    pressedColor = sf::Color(178, 178, 178);
}

void SpriteButton::setPressedColor(sf::Color color)
{
    pressedColor = color;
}

void SpriteButton::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}

void SpriteButton::setTexture(sf::Texture &texture)
{
    sprite.setTexture(texture);
}

void SpriteButton::setOrigin(sf::Vector2f origin)
{
    sprite.setOrigin(origin);
}

void SpriteButton::handleInput(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
            if (mouseOver)
                pressed = true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
            if (mouseOver && pressed)
                doAction = true;
            break;
        }
        pressed = false;
    }
}

void SpriteButton::update(sf::RenderWindow *window)
{
    if (isMouseOver(sprite, window))
        mouseOver = true;
    else
        mouseOver = false;

    if (pressed) 
        sprite.setColor(pressedColor);
    else if (mouseOver)
        sprite.setColor(pressedColor); //gotta change to hover color later
    else
        sprite.setColor(sf::Color::White);
}

void SpriteButton::render(sf::RenderWindow *window)
{
    window->draw(sprite);
}

void SpriteButton::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

sf::FloatRect SpriteButton::getGlobalBounds()
{
    return sprite.getGlobalBounds();
}

sf::FloatRect SpriteButton::getLocalBounds()
{
    return sprite.getLocalBounds();
}

bool SpriteButton::isDoAction()
{
    return doAction;
}

void SpriteButton::didAction()
{
    doAction = false;
}

bool SpriteButton::isMouseOver()
{
    return mouseOver;
}

bool SpriteButton::isMouseOver(sf::Sprite sprite, sf::RenderWindow *window)
{
    return sprite.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
}
