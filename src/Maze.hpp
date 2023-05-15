#pragma once
#include <SFML/Graphics.hpp>

#include "Tile.hpp"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdlib>

class Maze
{
public:
    Maze();
    Maze(int row, int col, sf::Vector2f size, sf::Vector2f position);

    void generateMaze();
    void solveMaze(int startX, int startY, int endX, int endY);

    void resize(int row, int col);

    void setPosition(sf::Vector2f position);
    void setColors(sf::Color tileColor, sf::Color wallColor, sf::Color outlineColor);
    void setSpeedFactor(int factor);

    void handleInput(sf::Event event);
    void update(sf::RenderWindow *window);
    void render(sf::RenderWindow* window);

private:
    int row, col, steps, minSteps;

    float mazeW, mazeH;

    float sleepTime;
    int speedFactor;

    bool reachedEnd;

    std::vector<std::vector<Tile>> maze;
    std::vector<std::vector<sf::RectangleShape>> footprints;

    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<bool>> correct_path;
    std::vector<std::vector<bool>> min_correct_path;

    sf::RectangleShape mazeBorder;
    sf::RectangleShape player;

    sf::Color borderColor, tileColor, wallColor;

    sf::Vector2f position;
    
    void init();

    void clearMaze();
    void generateMaze_helper(int i, int j);
    void solveMaze_helper(int startX, int startY, int endX, int endY);
    void showPlayer(int i, int j);
    void backTrackCheck(bool backTrack, int i, int j);
    void neighboursUnvisited(std::vector<std::vector<int>> &vec, int i, int j);

};