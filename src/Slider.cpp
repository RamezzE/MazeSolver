#include "Slider.hpp"

Slider::Slider()
{
    axisSize = sf::Vector2f(200, 10);
    handleSize = sf::Vector2f(20, 30);
    position = sf::Vector2f(0, 0);

    setAxisSize(axisSize);
    setHandleSize(handleSize);
    setPosition(position);

    mouseOver = pressed = false;

    axis.setFillColor(sf::Color(63, 63, 63));
    handle.setFillColor(sf::Color(192, 192, 192));
    progress.setFillColor(handle.getFillColor());

    if (!font.loadFromFile(FONT_PATH))
        std::cout << "Error loading font\n";

    characterSize = 20;

    text.setFont(font);
    text.setFillColor(sf::Color::White);
}

void Slider::setPosition(sf::Vector2f position)
{
    this->position = position;

    axis.setPosition(position);
    progress.setOrigin(axis.getOrigin());
    progress.setPosition(axis.getPosition());

    handle.setPosition(position.x + (axisSize.x / (maxValue - minValue)) * (sliderValue - minValue), position.y);
}

void Slider::setAxisSize(sf::Vector2f size)
{
    this->axisSize = size;
    axis.setSize(size);
    sf::FloatRect temp = axis.getLocalBounds();
    axis.setOrigin(temp.left, temp.top + temp.height / 2);

    handle.setPosition(position.x + (axisSize.x / (maxValue - minValue)) * (sliderValue - minValue), position.y);

    progress.setOrigin(axis.getOrigin());
    progress.setPosition(axis.getPosition());
    progress.setSize(sf::Vector2f(handle.getGlobalBounds().left - axis.getGlobalBounds().left, axis.getSize().y));
}

void Slider::setHandleSize(sf::Vector2f size)
{
    this->handleSize = size;
    handle.setSize(handleSize);
    sf::FloatRect temp = handle.getLocalBounds();
    handle.setOrigin(temp.left + temp.width / 2, temp.top + temp.height / 2);

    handle.setPosition(position.x + (axisSize.x / (maxValue - minValue)) * (sliderValue - minValue), position.y);
    progress.setSize(sf::Vector2f(handle.getGlobalBounds().left - axis.getGlobalBounds().left, axis.getSize().y));
}

void Slider::setColors(sf::Color axisColor, sf::Color handleColor)
{
    axis.setFillColor(axisColor);
    handle.setFillColor(handleColor);
    progress.setFillColor(handle.getFillColor());
}

void Slider::setCharacterSize(float size) {
    characterSize = size;
}

sf::Text Slider::returnText(int x, int y, std::string z, int fontSize)
{
    text.setCharacterSize(fontSize);
    text.setPosition(x, y);
    text.setString(z);
    return text;
}

void Slider::create(int min, int max)
{
    sliderValue = min;
    minValue = min;
    maxValue = max;
}

void Slider::handleInput(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        switch (event.mouseButton.button)
        {
        case sf::Mouse::Left:
            if (mouseOver)
                pressed = true;
            break;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
        pressed = false;
}

void Slider::update(sf::RenderWindow *window)
{

    if (isMouseOver(handle, window) || isMouseOver(axis, window))
        mouseOver = true;
    else
        mouseOver = false;

    if (pressed)
    {
        handle.setPosition(sf::Mouse::getPosition(*window).x, position.y);
        sliderValue = (minValue + ((handle.getPosition().x - position.x) / axisSize.x * (maxValue - minValue)));
    }

    if (handle.getPosition().x < position.x)
    {
        handle.setPosition(position.x, position.y);
        sliderValue = (minValue + ((handle.getPosition().x - position.x) / axisSize.x * (maxValue - minValue)));
    }
    else if (handle.getPosition().x > position.x + axisSize.x)
    {
        handle.setPosition(position.x + axisSize.x, position.y);
        sliderValue = (minValue + ((handle.getPosition().x - position.x) / axisSize.x * (maxValue - minValue)));
    }

    progress.setSize(sf::Vector2f(handle.getGlobalBounds().left - axis.getGlobalBounds().left, axis.getSize().y));
}

float Slider::getSliderValue()
{
    return sliderValue;
}

void Slider::setSliderValue(float newValue)
{
    if (newValue >= minValue && newValue <= maxValue)
    {
        sliderValue = newValue;
        float diff = maxValue - minValue;
        float diff2 = newValue - minValue;
        float zzz = axisSize.x / diff;
        float posX = zzz * diff2;
        posX += position.x;
        handle.setPosition(posX, position.y);
    }
}

void Slider::setSliderPercentValue(float newPercentValue)
{
    if (newPercentValue >= 0 && newPercentValue <= 100)
    {
        sliderValue = newPercentValue / 100 * maxValue;
        handle.setPosition(position.x + (axisSize.x * newPercentValue / 100), position.y);
    }
}

void Slider::render(sf::RenderWindow* window)
{
    window->draw(axis);

    if ((int)sliderValue != minValue)
        window->draw(returnText(position.x - 10, position.y + 5, std::to_string(minValue), characterSize));

    //round up

    if ((int)ceil(sliderValue) != maxValue)
        window->draw(returnText(position.x + axisSize.x - 10, position.y + 5, std::to_string(maxValue), characterSize));

    window->draw(progress);
    window->draw(handle);
    window->draw(returnText(handle.getPosition().x - handleSize.x, handle.getPosition().y - handleSize.y, std::to_string((int)sliderValue), characterSize));
}

bool Slider::isMouseOver(sf::RectangleShape sprite, sf::RenderWindow *window)
{
    return sprite.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
}

sf::FloatRect Slider::getGlobalBounds()
{
    return axis.getGlobalBounds();
}