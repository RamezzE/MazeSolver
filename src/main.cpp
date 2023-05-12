#include <iostream>
#include "GameScreen.hpp"
#include "Game.hpp"
#include "GameState.hpp"

const int N = 5;

template <class DataType>
int min(DataType &a, DataType &b)
{
    return a < b ? a : b;
}

bool visited[N][N] = {0};
bool correct_path[N][N] = {0};
bool min_correct_path[N][N] = {0};

int steps = -1;
int minSteps = 100000;

void solveMaze(const int arr[][N], int i, int j) // i & j are startX and startY
{
    if (arr[i][j] == 0)
        return;

    

    if (visited[N - 1][N - 1]) // end point
    {
        std::cout << "Valid Path: " << std::endl;
        for (int x = 0; x < N; x++)
        {
            for (int y = 0; y < N; y++)
                std::cout << correct_path[x][y] << " ";
            std::cout << std::endl;
        }
        std::cout << "Steps: " << steps << std::endl;

        std::cout << std::endl;

        if (steps < minSteps)
        {
            minSteps = steps;
            for (int x = 0; x < N; x++)
                for (int y = 0; y < N; y++)
                    min_correct_path[x][y] = correct_path[x][y];
        }

        // correct_path[N][N] = {0};
        return;
    }

    visited[i][j] = true;
    steps++;
    correct_path[i][j] = true;

    // the 4 adjacent cells
    // top
    if (i - 1 >= 0)
        if (!visited[i - 1][j])
            solveMaze(arr, i - 1, j);

    // bottom
    if (i + 1 < N)
        if (!visited[i + 1][j])
            solveMaze(arr, i + 1, j);

    // left
    if (j - 1 >= 0)
        if (!visited[i][j - 1])
            solveMaze(arr, i, j - 1);

    // right
    if (j + 1 < N)
        if (!visited[i][j + 1])
            solveMaze(arr, i, j + 1);

    visited[i][j] = false;
    steps--;
    correct_path[i][j] = false;
}

int main()
{
    int maze[N][N] = {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 1, 0, 1},
        {1, 1, 1, 1, 1}
    };
    
    std::cout << std::endl;

    std::cout << "Maze: " << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << maze[i][j] << " ";
        std::cout << std::endl;
    }

    std::cout << std::endl;

    solveMaze(maze, 0, 0);

    std::cout << "Best solution: " <<std::endl;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << min_correct_path[i][j] << " ";
        std::cout << std::endl;
    }
    std::cout << "Steps: " << minSteps << std::endl;

    Game myGame;
    myGame.changeScreen(new GameScreen(&myGame));
    myGame.gameLoop();
}