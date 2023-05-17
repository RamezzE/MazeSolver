#include "Button.hpp"

#include <iostream>
Button::Button()
{
    noTexture = true;
    init();
}

Button::Button(sf::Texture &texture)
{
    sprite.setTexture(texture);
    noTexture = false;
    init();
}

Button::Button(sf::Font &font)
{
    this->font = font;
    noTexture = true;
    init();
}

void Button::init()
{
    mouseOver = pressed = doAction = false;
    enabled = true;
    pressedColor = sf::Color(178, 178, 178);

    border.setFillColor(sf::Color::Transparent);

    text.setString("Button");
}

void Button::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

void Button::setFont(sf::Font &font)
{
    this->font = font;
    text.setFont(font);
}

void Button::setCharacterSize(int size)
{
    text.setCharacterSize(size);
    border.setSize(sf::Vector2f(text.getGlobalBounds().width * 1.1, size));
    sf::FloatRect temp = text.getGlobalBounds();
    text.setOrigin(temp.left + temp.width / 2, temp.top + temp.height / 2);
}

void Button::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
    text.setPosition(position);

    sf::FloatRect temp = text.getLocalBounds();
    text.setOrigin(temp.left + temp.width / 2, temp.top + temp.height / 2);

    temp = border.getLocalBounds();
    border.setOrigin(temp.left + temp.width / 2, temp.top + temp.height / 2);
    border.setPosition(position);
}

void Button::setText(std::string text, sf::Color color)
{
    this->text.setString(text);
    this->text.setFillColor(color);
    textColor = color;

    border.setSize(sf::Vector2f(this->text.getGlobalBounds().width * 1.1, this->text.getCharacterSize()));
}

void Button::setBorder(sf::Color color, int thickness)
{
    border.setOutlineThickness(thickness);
    borderColor = color;
    border.setOutlineColor(color);
}

void Button::setBackgroundColor(sf::Color color)
{
    backgroundColor = color;
    border.setFillColor(color);
}

void Button::setPressedColor(sf::Color color)
{
    pressedColor = color;
}

void Button::setScale(sf::Vector2f scale)
{
    sprite.setScale(scale);
}

void Button::setTexture(sf::Texture &texture)
{
    sprite.setTexture(texture);
    noTexture = false;
}

void Button::setOrigin(sf::Vector2f origin)
{
    sprite.setOrigin(origin);
}

void Button::handleInput(sf::Event event)
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
            if (mouseOver)
                doAction = true;
            break;
        }
        pressed = false;
    }
}

void Button::update(sf::RenderWindow *window)
{
    if (isMouseOver(border, window))
        mouseOver = true;
    else
        mouseOver = false;

    if (!noTexture)
    {
        if (isMouseOver(sprite, window))
            mouseOver = true;
        else
            mouseOver = false;
    }

    if (!enabled)
        mouseOver = pressed = doAction = false;

    if (mouseOver)
    {
        border.setOutlineColor(sf::Color::Magenta);
        border.setFillColor(sf::Color::Transparent);
    }
    else
    {
        border.setOutlineColor(borderColor);
        border.setFillColor(backgroundColor);
    }

    if (pressed)
    {
        text.setFillColor(backgroundColor);
        border.setFillColor(textColor);
        sprite.setColor(pressedColor);
    }
    else
    {
        text.setFillColor(textColor);
        border.setFillColor(backgroundColor);
        sprite.setColor(sf::Color::White);
    }
}

void Button::render(sf::RenderWindow *window)
{
    window->draw(border);
    window->draw(text);

    if (!enabled) {
        sf::RectangleShape disabledShade(border);
        disabledShade.setFillColor(sf::Color(0, 0, 0, 150));
        window->draw(disabledShade);
    }

    if (noTexture)
        return;

    window->draw(sprite);
}

sf::FloatRect Button::getGlobalBounds()
{
    if (noTexture)
        return border.getGlobalBounds();

    return sprite.getGlobalBounds();
}

sf::FloatRect Button::getLocalBounds()
{
    if (noTexture)
        return border.getGlobalBounds();

    return sprite.getLocalBounds();
}

bool Button::isDoAction()
{
    return doAction;
}

void Button::didAction()
{
    doAction = false;
}

bool Button::isMouseOver()
{
    return mouseOver;
}

bool Button::isMouseOver(sf::RectangleShape sprite, sf::RenderWindow *window)
{
    return sprite.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
}

bool Button::isMouseOver(sf::Sprite sprite, sf::RenderWindow *window)
{
    return sprite.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
}