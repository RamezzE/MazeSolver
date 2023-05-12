#pragma once
#include <SFML/Graphics.hpp>

#include "Cell.hpp"
#include <iostream>

namespace Maze {
    void solveMaze(std::vector<std::vector<Cell>> maze, int i, int j);

    void solveMaze_helper(std::vector<std::vector<Cell>> maze, int i, int j);
}