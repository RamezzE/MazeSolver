#pragma once
#include <SFML/Graphics.hpp>
#include "Definitions.hpp"

class SpriteButton
{
public:
    SpriteButton();
    SpriteButton(sf::Texture &texture);

    void handleInput(sf::Event event);
    void update(sf::RenderWindow *window);
    void render(sf::RenderWindow *window);

    void setOrigin(sf::Vector2f origin);
    void setTexture(sf::Texture &texture);
    void setScale(sf::Vector2f scale);
    void setPosition(sf::Vector2f position);
    void setPressedColor(sf::Color color);

    bool isDoAction();
    void didAction();
    bool isMouseOver();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();

private:
    void init();
    sf::Sprite sprite;

    bool mouseOver, pressed, doAction;

    bool isMouseOver(sf::Sprite sprite, sf::RenderWindow *window);
    sf::Color pressedColor;

};