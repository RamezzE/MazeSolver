#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#include "Definitions.hpp"

class Button
{
public:
    Button();
    Button(sf::Texture &texture);
    Button(sf::Font &font);

    void handleInput(sf::Event event);
    void update(sf::RenderWindow *window);
    void render(sf::RenderWindow *window);

    void setOrigin(sf::Vector2f origin);
    void setTexture(sf::Texture &texture);
    void setScale(sf::Vector2f scale);
    void setPosition(sf::Vector2f position);
    void setPressedColor(sf::Color color);

    void setFont(sf::Font &font);
    void setBorder(sf::Color color, int thickness);
    void setText(std::string text,sf::Color color);
    void setCharacterSize(int size);
    void setBackgroundColor(sf::Color color);
    void setBackground(sf::Texture &texture);

    bool isDoAction();
    void didAction();
    bool isMouseOver();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();

private:
    void init();

    bool isMouseOver(sf::RectangleShape sprite, sf::RenderWindow *window);

    bool isMouseOver(sf::Sprite sprite, sf::RenderWindow *window);

    sf::Sprite sprite;

    sf::RectangleShape border;
    sf::Font font;
    sf::Text text;

    bool mouseOver, pressed, doAction, noTexture;

    sf::Color pressedColor, backgroundColor, borderColor, textColor;
};