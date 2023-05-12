#include "Maze.hpp"

Maze::Maze() {}

Maze::Maze(int row, int col, sf::Vector2f size, sf::Vector2f position)
{
    this->row = row;
    this->col = col;

    mazeW = size.x;
    mazeH = size.y;

    maze.resize(row);
    for (int i = 0; i < row; i++)
        maze[i].resize(col);

    sleepTime = 50000;
    steps = -1;
    minSteps = 999999;

    init();
    setPosition(position);
}

void Maze::init()
{

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            maze[i][j] = Tile();
            maze[i][j].setSize(sf::Vector2f(mazeW / col, mazeH / row));

            if (i % 2 == 0)
            {
                maze[i][j].setWall(0, true);
                maze[i][j].setWall(1, true);
            }
        }

    mazeBorder.setOutlineColor(sf::Color(21, 23, 44));
    mazeBorder.setOutlineThickness(10);
    mazeBorder.setSize(sf::Vector2f(mazeW, mazeH));
    mazeBorder.setFillColor(sf::Color::Transparent);

    player.setFillColor(sf::Color::Magenta);
    player.setSize(sf::Vector2f(maze[0][0].getGlobalBounds().width / 2, maze[0][0].getGlobalBounds().height / 2));

    footprints.resize(row);
    for (int i = 0; i < row; i++)
        footprints[i].resize(col);

    for (int i = 0; i < footprints.size(); i++)
        for (int j = 0; j < footprints[0].size(); j++)
        {
            footprints[i][j] = sf::RectangleShape(player);
            footprints[i][j].setFillColor(sf::Color::Transparent);
        }
}

void Maze::setPosition(sf::Vector2f position)
{
    float posX = position.x;
    float posY = position.y;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            maze[i][j].setPosition(sf::Vector2f(j * mazeW / col + posX, i * mazeH / row + posY));
            footprints[i][j].setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 4, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 4);
        }

    mazeBorder.setPosition(maze[0][0].getPosition());
    player.setPosition(maze[0][0].getGlobalBounds().left + maze[0][0].getGlobalBounds().width / 4, maze[0][0].getGlobalBounds().top + maze[0][0].getGlobalBounds().height / 4);
}

void Maze::setColors(sf::Color tileColor, sf::Color wallColor, sf::Color outlineColor)
{
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            maze[i][j].setColor(tileColor, wallColor);

    mazeBorder.setOutlineColor(outlineColor);
}

void Maze::handleInput(sf::Event event)
{
    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].handleInput(event);
}

void Maze::update(sf::RenderWindow *window)
{
    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].update(window);
}

void Maze::render(sf::RenderWindow *window)
{

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
        {
            maze[i][j].renderTiles(window);
            window->draw(footprints[i][j]);
        }

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].renderLines(window);

    window->draw(player);
    window->draw(mazeBorder);
}

void Maze::solveMaze(int startX, int startY, int endX, int endY)
{
    steps = -1;
    minSteps = 999999;

    visited.assign(row, std::vector<bool>(col, false));
    correct_path.assign(row, std::vector<bool>(col, false));
    min_correct_path.assign(row, std::vector<bool>(col, false));

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            footprints[i][j].setFillColor(sf::Color::Transparent);

    steps = -1;
    minSteps = 999999;
    std::cout << "Solving Maze..." << std::endl;

    solveMaze_helper(startX, startY, endX, endY);

    for (int x = 0; x < maze.size(); x++)
        for (int y = 0; y < maze[0].size(); y++)
            if (min_correct_path[x][y])
                footprints[x][y].setFillColor(sf::Color(0, 255, 0, 100));

    std::cout << "Done" << std::endl;
}

void Maze::solveMaze_helper(int i, int j, int endX, int endY) // i & j are startX and startY
{
    backTrackCheck(false, i, j);

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

        backTrackCheck(true, i, j);

        return;
    }

    // the 4 adjacent tiles

    // right
    if (j + 1 < col)
    {
        if (!visited[i][j + 1] && !maze[i][j].getWall(1))
            solveMaze_helper(i, j + 1, endX, endY);
    }
    // bottom
    if (i + 1 < row)
    {
        if (!visited[i + 1][j] && !maze[i + 1][j].getWall(0))
            solveMaze_helper(i + 1, j, endX, endY);
    }

    // top
    if (i - 1 >= row)
    {
        if (!visited[i - 1][j] && !maze[i][j].getWall(0))
            solveMaze_helper(i - 1, j, endX, endY);
    }

    // left
    if (j - 1 >= col)
    {
        if (!visited[i][j - 1] && !maze[i][j - 1].getWall(1))
            solveMaze_helper(i, j - 1, endX, endY);
    }

    backTrackCheck(true, i, j);
}

void Maze::backTrackCheck(bool backTrack, int i, int j)
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
