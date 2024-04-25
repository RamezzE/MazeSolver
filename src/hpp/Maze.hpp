#pragma once
#include "../../SFML/include/SFML/Graphics.hpp"

#include "Tile.hpp"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <cstdlib>

class Tile;
class Maze
{
public:
    Maze();
    Maze(int row, int col, sf::Vector2f size, sf::Vector2f position);

    void resize(sf::Vector2f size);

    void generateMaze();
    void solveMaze();
    void findShortestPath();

    void resizeGrid(int row, int col);

    void setPosition(sf::Vector2f position);
    void setColors(sf::Color tileColor, sf::Color wallColor, sf::Color outlineColor);
    void setSpeedFactor(int factor);
    void setWallThicknessFactor(float factor);

    void handleInput(sf::Event event,sf::RenderWindow *window);
    void update(sf::RenderWindow *window);
    void render(sf::RenderWindow* window);
    void render(sf::RenderTexture* window);

    float getWallThicknessFactor();
    float getWallThicknessScale();
    
    bool threadRunning, choosingStartOrEnd, mazeGenerated, editMode, mouseClicked, pause, showStart, showEnd, showPath;

    int startX, startY, endX, endY;

    std::vector<std::vector<std::vector<bool>>> shortestPaths;
    int shortestPathIndex;

    void nextShortestPath();
    void prevShortestPath();

private:
    int row, col, steps, minSteps;

    float mazeW, mazeH;

    float sleepTime;
    int speedFactor, mouseClickCounter;

    bool reachedEnd, shortestPathAlgorithm;

    std::vector<std::vector<Tile>> maze;
    std::vector<std::vector<sf::RectangleShape>> footprints;

    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<bool>> correct_path;

    sf::RectangleShape mazeBorder;
    sf::RectangleShape player;

    sf::Color borderColor, tileColor, wallColor, correctPathColor, wrongPathColor, playerColor;

    sf::Vector2f position;
    
    void init();

    void clearMaze();
    void generateMaze_helper(int i, int j);
    void solveMaze_helper(int startX, int startY, int endX, int endY);

    void showPlayer(int i, int j);
    void backTrackCheck(bool backTrack, int i, int j);
    void neighboursUnvisited(std::vector<std::vector<int>> &vec, int i, int j);
    
    bool isMouseOver(Tile tile, sf::RenderWindow *window);
    bool isMouseOver(sf::RectangleShape shape, sf::RenderWindow *window);

    float thicknessScale, thicknessFactor;

};