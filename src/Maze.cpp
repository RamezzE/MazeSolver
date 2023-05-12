#include "Maze.hpp"

namespace Maze
{

    int steps = -1;
    int minSteps = 999999;

    float sleepTime = 50000;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<bool>> correct_path;
    std::vector<std::vector<bool>> min_correct_path;

    void XX(bool backTrack, std::vector<std::vector<Cell>> &maze, int i, int j, sf::RectangleShape &player, std::vector<std::vector<sf::RectangleShape>> &footprints)
    {

        visited[i][j] = !backTrack;
        correct_path[i][j] = !backTrack;

        if (backTrack)
        {
            steps--;
            footprints[i][j].setFillColor(sf::Color(255, 0, 0, 50));
        }
        else
        {
            steps++;
            footprints[i][j].setFillColor(sf::Color(100, 100, 100, 50));
        }
        player.setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 4, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 4);
        usleep(sleepTime);
    }

    void solveMaze(std::vector<std::vector<Cell>> &maze, int startX, int startY, int endX, int endY, sf::RectangleShape &player, std::vector<std::vector<sf::RectangleShape>> &footprints)
    {
        int N = maze.size();

        visited.assign(N, std::vector<bool>(N, false));
        correct_path.assign(N, std::vector<bool>(N, false));
        min_correct_path.assign(N, std::vector<bool>(N, false));

        for (int x = 0; x < N; x++)
            for (int y = 0; y < N; y++)
                footprints[x][y].setFillColor(sf::Color::Transparent);

        steps = -1;
        minSteps = 999999;
        endX = N - 1, endY = N - 1;
        std::cout << "Solving Maze..." << std::endl;

        solveMaze_helper(std::ref(maze), startX, startY, endX, endY, player, footprints);

        for (int x = 0; x < maze.size(); x++)
            for (int y = 0; y < maze[0].size(); y++)
            {
                if (min_correct_path[x][y])
                    footprints[x][y].setFillColor(sf::Color::Green);
            }

        std::cout << "Done" << std::endl;
    }

    void solveMaze_helper(std::vector<std::vector<Cell>> &maze, int i, int j, int endX, int endY, sf::RectangleShape &player, std::vector<std::vector<sf::RectangleShape>> &footprints) // i & j are startX and startY
    {
        int N = maze.size();

        XX(false, maze, i, j, player, footprints);

        if (visited[endX][endY]) // end point
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
                for (int x = 0; x < maze.size(); x++)
                    for (int y = 0; y < maze[0].size(); y++)
                        min_correct_path[x][y] = correct_path[x][y];

                for (int x = 0; x < maze.size(); x++)
                {
                    for (int y = 0; y < maze[0].size(); y++)
                        std::cout << min_correct_path[x][y] << " ";

                    std::cout << std::endl;
                }
                std::cout << minSteps << std::endl;
                std::cout << std::endl;
            }

            XX(true, maze, i, j, player, footprints);

            usleep(sleepTime);
            return;
        }

        // the 4 adjacent cells

        // right
        if (j + 1 < N)
        {
            if (!visited[i][j + 1] && !maze[i][j].getWall(1))
                solveMaze_helper(maze, i, j + 1, endX, endY, player, footprints);
        }
        // bottom
        if (i + 1 < N)
        {
            if (!visited[i + 1][j] && !maze[i + 1][j].getWall(0))
                solveMaze_helper(maze, i + 1, j, endX, endY, player, footprints);
        }

        // top
        if (i - 1 >= 0)
        {
            if (!visited[i - 1][j] && !maze[i][j].getWall(0))
                solveMaze_helper(maze, i - 1, j, endX, endY,player, footprints);
        }

        // left
        if (j - 1 >= 0)
        {
            if (!visited[i][j - 1] && !maze[i][j - 1].getWall(1))
                solveMaze_helper(maze, i, j - 1, endX, endY,player, footprints);
        }

        XX(true, maze, i, j, player, footprints);
    }

}