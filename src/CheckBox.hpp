#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#include "Definitions.hpp"

class CheckBox
{
public:
    CheckBox();

    void handleInput(sf::Event event);
    void update(sf::RenderWindow *window);
    void render(sf::RenderWindow *window);

    void setSize(sf::Vector2f size);
    void setPosition(sf::Vector2f position);
    void setCheckedColor(sf::Color color);
    void setEnabled(bool enabled);

    void setBorder(sf::Color color, int thickness);
    void setBackgroundColor(sf::Color color);
    void setBackground(sf::Texture &texture);

    bool isChecked();
    bool isMouseOver();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
    sf::Vector2f getPosition();
    sf::Vector2f getSize();

private:
    void init();

    bool isMouseOver(sf::RectangleShape sprite, sf::RenderWindow *window);

    sf::RectangleShape border;

    bool mouseOver, pressed, checked , enabled;

    sf::Color checkedColor, backgroundColor, borderColor, hoverColor;
};