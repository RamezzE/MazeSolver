#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

typedef unsigned short ushort;

class TextBox
{
public:
    TextBox(sf::Font &font);

    void handleInput(sf::Event event);

    void update(sf::RenderWindow *window);

    void draw(sf::RenderWindow *window);

    void setSize(sf::Vector2f size);

    void setPosition(sf::Vector2f position);

    void setOrigin(sf::Vector2f origin);

    void setBorder(float thickness, sf::Color color, sf::Color hoverColor, sf::Color selectedColor);

    void setTextFormat(sf::Color color, ushort size);
    void setTextColor(sf::Color color);

    void setTextLimit(int limit);

    void setString(std::string str);

    void setBackgroundColor(sf::Color color);

    void setSelected(bool selected);

    void allowNumberOnly();

    void allowAlphaOnly();

    void allowAlphaNumeric();

    bool isHovered();

    bool isSelected();

    sf::Vector2f getPosition();

    sf::FloatRect getGlobalBounds();

    std::string getString();

    int getCharacterSize();

private:
    sf::RectangleShape border;
    sf::Text text;
    sf::Font font;

    sf::Vector2f position;
    sf::Vector2f origin;

    int textLimit, subsetCounter, maxCharactersDisplayed;

    bool selected, hover, validTextEntered;

    std::string inputString;

    sf::Color borderColor, borderHoverColor, borderSelectedColor;

    ushort typeAllowed;

    enum
    {
        BACKSPACE = 8,
        SPACE = 32,
        PERIOD = 46,
        ZERO = 48,
        NINE = 57,
        A = 65,
        Z = 90,
        A_CAPS = 97,
        Z_CAPS = 122
    };

    enum {
        NUMBER_ONLY,
        ALPHA_ONLY,
        ALPHA_NUMERIC
    };

    bool isMouseOver(TextBox textbox, sf::RenderWindow *window);
    float getCharacterWidth();
    int calculateMaxCharactersDisplayed();

};