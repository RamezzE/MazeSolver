#include "Maze.hpp"

namespace Maze
{

    const int N = 5;
    bool visited[N][N] = {false};
    bool correct_path[N][N] = {false};
    bool min_correct_path[N][N] = {false};

    int steps = -1;
    int minSteps = 999999;
    int startX , startY , endX = N - 1, endY = N - 1;

    void solveMaze(std::vector<std::vector<Cell>> maze, int i, int j) {

        for (int x = 0; x < N; x++) {
            for (int y = 0; y < N; y++) {
                visited[x][y] = false;
                correct_path[x][y] = false;
                min_correct_path[x][y] = false;
            }
        }

        steps = -1;
        minSteps = 999999;
        endX = N - 1, endY = N - 1;

        solveMaze_helper(maze, i, j);
    }

    void solveMaze_helper(std::vector<std::vector<Cell>> maze, int i, int j) // i & j are startX and startY
    {
        // std::cout << "i: " << i << " j: " << j << std::endl;
        if (i == endX && j == endY)
        {
            visited[i][j] = true;
            steps++;
            correct_path[i][j] = true;
        }

        if (i == endX && j == endY) // end point
        {
            // std::cout << "Valid Path: " << std::endl;
            // for (int x = 0; x < N; x++)
            // {
            //     for (int y = 0; y < N; y++)
            //         std::cout << correct_path[x][y] << " ";
            //     std::cout << std::endl;
            // }

            if (steps < minSteps)
            {
                minSteps = steps;
                for (int x = 0; x < N; x++)
                    for (int y = 0; y < N; y++)
                        min_correct_path[x][y] = correct_path[x][y];

                for (int x = 0; x < N; x++)
                {
                    for (int y = 0; y < N; y++)
                        std::cout << min_correct_path[x][y] << " ";

                    std::cout << std::endl;
                }
                std::cout << minSteps << std::endl;
                std::cout << std::endl;

            }

            correct_path[N][N] = {0};
            return;
        }

        visited[i][j] = true;
        steps++;
        correct_path[i][j] = true;

        // the 4 adjacent cells

        // right
        if (j + 1 < N)
        {
            if (!visited[i][j + 1] && !maze[i][j].getWall(1)) {
                solveMaze_helper(maze, i, j + 1);
            }
        }
        // bottom
        if (i + 1 < N)
        {
            if (!visited[i + 1][j] && !maze[i + 1][j].getWall(0))
            {
                solveMaze_helper(maze, i + 1, j);
            }
        }


        // top
        if (i - 1 >= 0)
        {
            if (!visited[i - 1][j] && !maze[i][j].getWall(0))
            {
                solveMaze_helper(maze, i - 1, j);
            }
        }
        
        // left
        if (j - 1 >= 0)
        {
            if (!visited[i][j - 1] && !maze[i][j - 1].getWall(1)) {
                solveMaze_helper(maze, i, j - 1);
            }
        }
        

        visited[i][j] = false;
        steps--;
        correct_path[i][j] = false;
    }
}