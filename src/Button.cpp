#include "Button.hpp"

#include <iostream>
Button::Button()
{
    init();
}

Button::Button(sf::Font &font)
{
    this->font = font;
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
    border.setSize(sf::Vector2f(text.getGlobalBounds().width * 1.1, text.getGlobalBounds().height * 1.2));
    sf::FloatRect temp = text.getGlobalBounds();
    text.setOrigin(temp.left + temp.width / 2, temp.top + temp.height / 2);
}

void Button::setPosition(sf::Vector2f position)
{
    text.setPosition(position);

    sf::FloatRect temp = text.getLocalBounds();
    text.setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f);

    temp = border.getLocalBounds();
    border.setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f);
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
    }
    else
    {
        text.setFillColor(textColor);
        border.setFillColor(backgroundColor);
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
}

sf::FloatRect Button::getGlobalBounds()
{
    return border.getGlobalBounds();
}

sf::FloatRect Button::getLocalBounds()
{
    return border.getLocalBounds();
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