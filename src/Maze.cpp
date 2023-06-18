#include "Maze.hpp"

Maze::Maze(int row, int col, sf::Vector2f size, sf::Vector2f position)
{
    srand(time(NULL));
    this->row = row;
    this->col = col;
    this->position = position;

    mazeW = size.x;
    mazeH = size.y;

    sleepTime = 500000;
    steps = -1;
    minSteps = 999999;

    speedFactor = 1;

    threadRunning = choosingStartOrEnd = mazeGenerated = editMode = pause = showStart = showEnd = showPath = false;

    startX = startY = mouseClickCounter = 0;

    endX = row - 1;
    endY = col - 1;

    correctPathColor = sf::Color::Green;
    wrongPathColor = sf::Color(23, 28, 116);
    playerColor = sf::Color(23, 165, 229);

    thicknessScale = 0.0015f;
    thicknessFactor = 1;

    resizeGrid(row, col);

    footprints[endX][endY].setFillColor(sf::Color::White);
}

void Maze::init()
{
    maze.resize(row);
    for (int i = 0; i < row; i++)
        maze[i].resize(col);

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            maze[i][j] = Tile(this);
            maze[i][j].setSize(sf::Vector2f(mazeW / col, mazeH / row));
        }

    mazeBorder.setOutlineThickness(10);
    mazeBorder.setSize(sf::Vector2f(mazeW, mazeH));
    mazeBorder.setFillColor(sf::Color::Transparent);

    player.setFillColor(playerColor);
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

    footprints[endX][endY].setFillColor(sf::Color::White);
}

void Maze::resize(sf::Vector2f size)
{
    mazeW = size.x;
    mazeH = size.y;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            maze[i][j].setSize(sf::Vector2f(mazeW / col, mazeH / row));

    player.setSize(sf::Vector2f(maze[0][0].getGlobalBounds().width / 2, maze[0][0].getGlobalBounds().height / 2));
    for (int i = 0; i < footprints.size(); i++)
        for (int j = 0; j < footprints[0].size(); j++)
            footprints[i][j].setSize(sf::Vector2f(player.getSize().x / 2, player.getSize().y / 2));

    mazeBorder.setSize(size);
    setPosition(position);
}

void Maze::setPosition(sf::Vector2f position)
{
    this->position = position;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            maze[i][j].setPosition(sf::Vector2f(j * mazeW / col + position.x, i * mazeH / row + position.y));
            footprints[i][j].setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 2.5, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 2.5);
        }

    mazeBorder.setPosition(maze[0][0].getPosition());
    player.setPosition(maze[startX][startY].getGlobalBounds().left + maze[startX][startY].getGlobalBounds().width / 4, maze[startX][startY].getGlobalBounds().top + maze[startX][startY].getGlobalBounds().height / 4);
}

void Maze::setColors(sf::Color tileColor, sf::Color wallColor, sf::Color outlineColor)
{
    this->tileColor = tileColor;
    this->wallColor = wallColor;

    mazeBorder.setOutlineColor(outlineColor);

    if (!mazeGenerated)
        return;

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].setColor(tileColor, wallColor);
}

void Maze::setSpeedFactor(int factor)
{
    if (factor == 0)
        this->speedFactor = INT_MAX;
    else
        this->speedFactor = factor;
}

void Maze::setWallThicknessFactor(float factor)
{
    thicknessFactor = factor;
    float thickness = thicknessFactor * thicknessScale * mazeH;

    if (thickness < 1)
        thickness = 1;

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].setWallThickness(thickness);
}

void Maze::resizeGrid(int row, int col)
{
    for (int i = 0; i < maze.size(); i++)
        maze[i].clear();
    maze.clear();

    this->row = row;
    this->col = col;

    startX = startY = 0;

    endX = row - 1;
    endY = col - 1;

    init();
    setPosition(position);
    clearMaze();
}

void Maze::handleInput(sf::Event event, sf::RenderWindow *window)
{
    if (event.type == sf::Event::MouseButtonPressed)
        if (isMouseOver(mazeBorder, window))
            mouseClicked = true;

    if (event.type == sf::Event::MouseButtonReleased)
        mouseClicked = false;

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].handleInput(event);
}

void Maze::update(sf::RenderWindow *window)
{
    if (threadRunning)
        return;

    if (choosingStartOrEnd)
    {
        if (!mouseClicked)
            return;

        mouseClicked = editMode = false;

        if (mouseClickCounter == 0)
        {
            visited.assign(row, std::vector<bool>(col, false));
            correct_path.assign(row, std::vector<bool>(col, false));
            min_correct_path.assign(row, std::vector<bool>(col, false));

            for (int i = 0; i < row && i != endX; i++)
                for (int j = 0; j < col && j != endY; j++)
                    footprints[i][j].setFillColor(sf::Color::Transparent);
            
            for (int i = 0; i < maze.size(); i++)
                for (int j = 0; j < maze[i].size(); j++)
                    if (isMouseOver(maze[i][j], window))
                    {
                        player.setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 4, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 4);
                        startX = i;
                        startY = j;
                        mouseClickCounter++;
                        return;
                    }

            return;
        }

        for (int i = 0; i < maze.size(); i++)
            for (int j = 0; j < maze[i].size(); j++)
                if (isMouseOver(maze[i][j], window))
                {
                    footprints[endX][endY].setFillColor(sf::Color::Transparent);
                    endX = i;
                    endY = j;
                    footprints[endX][endY].setFillColor(sf::Color::White);
                    choosingStartOrEnd = false;
                    mouseClickCounter = 0;
                    return;
                }

        return;
    }

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

void Maze::render(sf::RenderTexture *window)
{
    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].renderTiles(window);

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].renderLines(window);

    if (showPath)
    {
        if (min_correct_path.size() > 0)
        {
            for (int x = 0; x < maze.size(); x++)
                for (int y = 0; y < maze[0].size(); y++)
                    if (min_correct_path[x][y])
                    {
                        footprints[x][y].setFillColor(sf::Color(wallColor.r, wallColor.g, wallColor.b, 100));
                        window->draw(footprints[x][y]);
                    }
        }
    }

    footprints[endX][endY].setFillColor(wallColor);
    player.setFillColor(wallColor);

    if (showStart)
        window->draw(player);

    if (showEnd)
        window->draw(footprints[endX][endY]);

    window->draw(mazeBorder);
}

void Maze::clearMaze()
{
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            maze[i][j].resetColor();
            footprints[i][j].setFillColor(sf::Color::Transparent);

            maze[i][j].setWall(0, true);
            maze[i][j].setWall(1, true);

            if (i == 0)
                maze[i][j].setWall(0, false);

            if (j == col - 1)
                maze[i][j].setWall(1, false);
        }

    mazeGenerated = false;
    visited.assign(row, std::vector<bool>(col, false));
    correct_path.assign(row, std::vector<bool>(col, false));
    min_correct_path.assign(row, std::vector<bool>(col, false));
}

void Maze::generateMaze()
{
    threadRunning = true;
    clearMaze();

    visited.assign(row, std::vector<bool>(col, false));

    std::cout << "Generating Maze..." << std::endl;

    srand(time(NULL));

    int i = rand() % row;
    int j = rand() % col;

    generateMaze_helper(i, j);

    std::cout << "Done Generating" << std::endl;
    threadRunning = false;
    mazeGenerated = true;
    i = startX, j = startY;
    player.setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 4, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 4);
    footprints[endX][endY].setFillColor(sf::Color::White);
}

void Maze::generateMaze_helper(int i, int j)
{
    while (true)
    {
        while (pause)
            usleep(10000);

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

void Maze::solveMaze()
{
    threadRunning = true;
    steps = -1;
    minSteps = 999999;
    reachedEnd = false;
    shortestPathAlgorithm = false;

    visited.assign(row, std::vector<bool>(col, false));
    correct_path.assign(row, std::vector<bool>(col, false));
    min_correct_path.assign(row, std::vector<bool>(col, false));

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            if (i == endX && j == endY)
            {
                footprints[i][j].setFillColor(sf::Color::White);
                continue;
            }
            footprints[i][j].setFillColor(sf::Color::Transparent);
        }

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

    if (footprints[endX][endY].getFillColor() == sf::Color::Transparent)
        footprints[endX][endY].setFillColor(sf::Color::White);

    std::cout << "Done" << std::endl;
    threadRunning = false;
}

void Maze::solveMaze_helper(int i, int j, int endX, int endY) // i & j are startX and startY
{
    while (pause)
        usleep(1000);

    showPlayer(i, j);
    backTrackCheck(false, i, j);

    if (!shortestPathAlgorithm && reachedEnd)
        return;
    else if (steps > minSteps)
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
        if (!shortestPathAlgorithm)
            reachedEnd = true;

        showPlayer(i, j);
        backTrackCheck(true, i, j);
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

void Maze::findShortestPath()
{
    shortestPathAlgorithm = true;
    threadRunning = true;
    steps = -1;
    minSteps = 999999;
    reachedEnd = false;

    visited.assign(row, std::vector<bool>(col, false));
    correct_path.assign(row, std::vector<bool>(col, false));
    min_correct_path.assign(row, std::vector<bool>(col, false));

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
        {
            if (i == endX && j == endY)
            {
                footprints[i][j].setFillColor(sf::Color::White);
                continue;
            }
            footprints[i][j].setFillColor(sf::Color::Transparent);
        }

    steps = -1;
    minSteps = 999999;
    std::cout << "Finding shortest path..." << std::endl;

    solveMaze_helper(startX, startY, endX, endY);

    for (int x = 0; x < maze.size(); x++)
        for (int y = 0; y < maze[0].size(); y++)
            if (min_correct_path[x][y])
                footprints[x][y].setFillColor(sf::Color(0, 255, 0, 255));
            else
                footprints[x][y].setFillColor(sf::Color::Transparent);

    if (footprints[endX][endY].getFillColor() == sf::Color::Transparent)
        footprints[endX][endY].setFillColor(sf::Color::White);

    std::cout << "Done" << std::endl;
    threadRunning = false;
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
            footprints[i][j].setFillColor(correctPathColor);
        else
            footprints[i][j].setFillColor(wrongPathColor);
    }
    else
    {
        steps++;
        footprints[i][j].setFillColor(player.getFillColor());
    }
}

bool Maze::isMouseOver(Tile tile, sf::RenderWindow *window)
{
    return tile.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
}

bool Maze::isMouseOver(sf::RectangleShape shape, sf::RenderWindow *window)
{
    return shape.getGlobalBounds().contains(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
}

float Maze::getWallThicknessFactor()
{
    return thicknessFactor;
}

float Maze::getWallThicknessScale()
{
    return thicknessScale;
}