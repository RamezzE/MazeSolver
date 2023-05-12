#pragma once
#include <SFML/Graphics.hpp>

#include "Cell.hpp"
#include <iostream>
#include <thread>
#include <unistd.h>

class Maze
{
public:
    Maze();
    Maze(int row, int col, sf::Vector2f size, sf::Vector2f position);

    void solveMaze(int startX, int startY, int endX, int endY);

    void setPosition(sf::Vector2f position);
    void setColors(sf::Color tileColor, sf::Color wallColor, sf::Color outlineColor);

    void handleInput(sf::Event event);
    void update(sf::RenderWindow *window);
    void render(sf::RenderWindow* window);

private:
    int mazeW, mazeH, row, col, steps, minSteps;

    float sleepTime;

    std::vector<std::vector<Cell>> maze;
    std::vector<std::vector<sf::RectangleShape>> footprints;

    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<bool>> correct_path;
    std::vector<std::vector<bool>> min_correct_path;

    sf::RectangleShape mazeBorder;
    sf::RectangleShape player;

    sf::Color borderColor, tileColor;
    
    void init();

    void solveMaze_helper(int startX, int startY, int endX, int endY);
    void backTrackCheck(bool backTrack, int i, int j);

};