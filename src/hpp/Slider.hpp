#pragma once
#include "../../SFML/include/SFML/Graphics.hpp"
#include <iostream>
#include <cmath>

#include "Definitions.hpp"

class Slider
{
public:
    Slider();

    void handleInput(sf::Event event);
    void update(sf::RenderWindow *window);
    void render(sf::RenderWindow *window);

    void setPosition(sf::Vector2f position);
    void setAxisSize(sf::Vector2f size);
    void setHandleSize(sf::Vector2f size);
    void setColors(sf::Color axisColor, sf::Color handleColor);

    void setCharacterSize(float size);

    void create(int min, int max);
    void setSliderValue(float newValue);
    void setSliderPercentValue(float newPercentValue);
    void draw(sf::RenderWindow &window);

    sf::Text returnText(int x, int y, std::string z, int fontSize);
    float getSliderValue();
    sf::FloatRect getGlobalBounds();

private:
    bool pressed, mouseOver;

    sf::RectangleShape handle, axis, progress;

    sf::Font font;
    sf::Text text;

    int minValue, maxValue;

    sf::Vector2f position, axisSize, handleSize;

    float sliderValue, characterSize;

    bool isMouseOver(sf::RectangleShape sprite, sf::RenderWindow *window);
};
