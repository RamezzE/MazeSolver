#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

#include "Definitions.hpp"

class Slider
{
public:
    Slider();

    void handleInput(sf::Event event);
    void update(sf::RenderWindow* window);
    void render(sf::RenderWindow& window);

    void setPosition(sf::Vector2f position);
    void setAxisSize(sf::Vector2f size);
    void setHandleSize(sf::Vector2f size);

    sf::Text returnText(int x, int y, std::string z, int fontSize);
    void create(int min, int max);
    float getSliderValue();
    void setSliderValue(float newValue);
    void setSliderPercentValue(float newPercentValue);
    void draw(sf::RenderWindow &window);

    sf::FloatRect getGlobalBounds();


private:

    bool pressed;
    bool mouseOver;

    sf::RectangleShape handle;
    sf::RectangleShape axis;

    sf::Font font;
    sf::Text text;

    int minValue;
    int maxValue;

    sf::Vector2f position;
    sf::Vector2f axisSize;
    sf::Vector2f handleSize;

    float sliderValue;

    bool isMouseOver(sf::RectangleShape sprite, sf::RenderWindow *window);
};