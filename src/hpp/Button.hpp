#pragma once
#include "../../SFML/include/SFML/Graphics.hpp"
#include <string>

#include "Definitions.hpp"

class Button
{
public:
    Button();
    Button(sf::Font &font);
    
    void handleInput(sf::Event event);
    void update(sf::RenderWindow *window);
    void render(sf::RenderWindow *window);

    void setPosition(sf::Vector2f position);
    void setPressedColor(sf::Color color);
    void setEnabled(bool enabled);

    void setFont(sf::Font &font);
    void setBorder(sf::Color color, int thickness);
    void setText(std::string text,sf::Color color);
    void setCharacterSize(int size);
    void setTextColor(sf::Color color);
    void setBackgroundColor(sf::Color color);
    void setBackground(sf::Texture &texture);

    bool isDoAction();
    void didAction();
    bool isMouseOver();

    sf::FloatRect getGlobalBounds();
    sf::FloatRect getLocalBounds();
    sf::Vector2f getPosition();
    sf::Color getTextColor();

private:
    void init();

    bool isMouseOver(sf::RectangleShape sprite, sf::RenderWindow *window);

    sf::RectangleShape border;
    sf::Font font;
    sf::Text text;

    bool mouseOver, pressed, doAction, enabled;

    sf::Color pressedColor, backgroundColor, borderColor, textColor, hoverColor;
};