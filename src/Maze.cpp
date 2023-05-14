#include "Maze.hpp"

Maze::Maze(int row, int col, sf::Vector2f size, sf::Vector2f position)
{
    srand(time(NULL));
    this->row = row;
    this->col = col;

    mazeW = size.x;
    mazeH = size.y;

    maze.resize(row);
    for (int i = 0; i < row; i++)
        maze[i].resize(col);

    sleepTime = 500000;
    steps = -1;
    minSteps = 999999;

    speedFactor = 1;

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
        }

    mazeBorder.setOutlineColor(sf::Color(21, 23, 44));
    mazeBorder.setOutlineThickness(10);
    mazeBorder.setSize(sf::Vector2f(mazeW, mazeH));
    mazeBorder.setFillColor(sf::Color::Transparent);

    player.setFillColor(sf::Color(23, 165, 229));
    player.setSize(sf::Vector2f(maze[0][0].getGlobalBounds().width / 2, maze[0][0].getGlobalBounds().height / 2));

    footprints.resize(row);
    for (int i = 0; i < row; i++)
        footprints[i].resize(col);

    for (int i = 0; i < footprints.size(); i++)
        for (int j = 0; j < footprints[0].size(); j++)
        {
            footprints[i][j] = sf::RectangleShape(player);
            footprints[i][j].setFillColor(sf::Color::Transparent);
            footprints[i][j].setSize(sf::Vector2f(player.getSize().x / 2, player.getSize().y / 2));
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
            footprints[i][j].setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 2.5, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 2.5);
        }

    mazeBorder.setPosition(maze[0][0].getPosition());
    player.setPosition(maze[0][0].getGlobalBounds().left + maze[0][0].getGlobalBounds().width / 4, maze[0][0].getGlobalBounds().top + maze[0][0].getGlobalBounds().height / 4);
}

void Maze::setColors(sf::Color tileColor, sf::Color wallColor, sf::Color outlineColor)
{
    this->tileColor = tileColor;
    this->wallColor = wallColor;

    mazeBorder.setOutlineColor(outlineColor);
}

void Maze::setSpeedFactor(int factor)
{
    if (factor == 0)
    {
        this->speedFactor = INT_MAX;
        return;
    }
    this->speedFactor = factor;
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

void Maze::clearMaze()
{
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            maze[i][j].setColor(sf::Color::White, tileColor);
            footprints[i][j].setFillColor(sf::Color::Transparent);

            maze[i][j].setWall(0, true);
            maze[i][j].setWall(1, true);

            if (i == 0)
                maze[i][j].setWall(0, false);

            if (j == col - 1)
                maze[i][j].setWall(1, false);
        }
}

void Maze::generateMaze()
{
    clearMaze();

    visited.assign(row, std::vector<bool>(col, false));

    std::cout << "Generating Maze..." << std::endl;

    srand(time(NULL));

    int i = rand() % row;
    int j = rand() % col;

    generateMaze_helper(i, j);

    std::cout << "Done Generating" << std::endl;
}

void Maze::generateMaze_helper(int i, int j)
{
    while (true)
    {
        usleep(sleepTime / speedFactor);
        player.setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 4, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 4);
        maze[i][j].setColor(tileColor, sf::Color::Magenta);
        visited[i][j] = true;

        std::vector<std::vector<int>> vec;

        neighboursUnvisited(vec, i, j);

        int r;
        if (vec.empty())
            return;

        r = rand() % vec.size();

        int nbrI = vec[r][0];
        int nbrJ = vec[r][1];

        int wall;

        int nbr2I = nbrI, nbr2J = nbrJ;

        if (nbrI == i)
        {
            wall = 1;
            nbr2I = i;
            if (nbrJ == j + 1)
                nbr2J = j;
        }
        else
        {
            wall = 0;
            nbr2J = j;
            if (nbrI == i - 1)
                nbr2I = i;
        }

        maze[nbr2I][nbr2J].setWall(wall, false);
        generateMaze_helper(nbrI, nbrJ);
    }
}

void Maze::neighboursUnvisited(std::vector<std::vector<int>> &vec, int i, int j)
{
    int x = 0;

    // bottom
    if (i + 1 < row)
        if (!visited[i + 1][j])
        {
            vec.push_back(std::vector<int>());
            vec[x].push_back(i + 1);
            vec[x++].push_back(j);
        }

    // top
    if (i - 1 >= 0)
        if (!visited[i - 1][j])
        {
            vec.push_back(std::vector<int>());
            vec[x].push_back(i - 1);
            vec[x++].push_back(j);
        }

    // right
    if (j + 1 < col)
        if (!visited[i][j + 1])
        {
            vec.push_back(std::vector<int>());
            vec[x].push_back(i);
            vec[x++].push_back(j + 1);
        }

    // left
    if (j - 1 >= 0)
        if (!visited[i][j - 1])
        {
            vec.push_back(std::vector<int>());
            vec[x].push_back(i);
            vec[x++].push_back(j - 1);
        }
}

void Maze::solveMaze(int startX, int startY, int endX, int endY)
{
    steps = -1;
    minSteps = 999999;
    reachedEnd = false;

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
                footprints[x][y].setFillColor(sf::Color(0, 255, 0, 255));
            else
                footprints[x][y].setFillColor(sf::Color::Transparent);

    std::cout << "Done" << std::endl;
}

void Maze::solveMaze_helper(int i, int j, int endX, int endY) // i & j are startX and startY
{
    showPlayer(i, j);
    backTrackCheck(false,i,j);

    if (reachedEnd)
        return;

    if (visited[endX][endY]) // end point
    {
        if (steps < minSteps)
        {
            minSteps = steps;
            for (int x = 0; x < maze.size(); x++)
                for (int y = 0; y < maze[0].size(); y++)
                    min_correct_path[x][y] = correct_path[x][y];
        }
        reachedEnd = true;
        showPlayer(i, j);
        backTrackCheck(true,i,j);
        return;
    }

    // the 4 adjacent tiles
    // right
    if (j + 1 < col)
    {
        if (!visited[i][j + 1] && !maze[i][j].getWall(1) && !reachedEnd)
        {
            solveMaze_helper(i, j + 1, endX, endY);
            showPlayer(i, j);
        }
    }
    // bottom
    if (i + 1 < row)
    {
        if (!visited[i + 1][j] && !maze[i + 1][j].getWall(0) && !reachedEnd)
        {
            solveMaze_helper(i + 1, j, endX, endY);
            showPlayer(i, j);
        }
    }

    // top
    if (i - 1 >= 0)
    {
        if (!visited[i - 1][j] && !maze[i][j].getWall(0) && !reachedEnd)
        {
            solveMaze_helper(i - 1, j, endX, endY);
            showPlayer(i, j);
        }
    }

    // left
    if (j - 1 >= 0)
    {
        if (!visited[i][j - 1] && !maze[i][j - 1].getWall(1) && !reachedEnd)
        {
            solveMaze_helper(i, j - 1, endX, endY);
            showPlayer(i, j);
        }
    }

    backTrackCheck(true, i, j);
}

void Maze::showPlayer(int i, int j)
{
    player.setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 4, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 4);
    usleep(sleepTime / speedFactor);
}

void Maze::backTrackCheck(bool backTrack, int i, int j)
{
    visited[i][j] = !backTrack;
    correct_path[i][j] = !backTrack;

    if (backTrack)
    {
        steps--;
        if (reachedEnd)
            footprints[i][j].setFillColor(sf::Color(0, 255, 0));
        else
            footprints[i][j].setFillColor(sf::Color(23, 28, 116));
    }
    else
    {
        steps++;
        footprints[i][j].setFillColor(player.getFillColor());
    }
}
