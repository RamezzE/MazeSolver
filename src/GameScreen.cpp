#include "GameScreen.hpp"

GameScreen::GameScreen(Game *myGame)
{
    this->game = myGame;
    init();
}

void GameScreen::init()
{
    int num = 10;
    maze.resize(num);
    for (int i = 0; i < num; i++)
        maze[i].resize(num);

    int w, h;
    w = game->width / num / 2;
    h = game->height / num / 2;

    mazeBorder.setOutlineColor(sf::Color::White);
    mazeBorder.setOutlineThickness(5);
    mazeBorder.setSize(sf::Vector2f(w * num, h * num));
    mazeBorder.setFillColor(sf::Color::Transparent);

    /*
        {1, 1, 1, 1, 1},
        {1, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 1, 0, 1},
        {1, 1, 1, 1, 1}
    */

    for (int i = 0; i < num; i++)
        for (int j = 0; j < maze[i].size(); j++)
        {
            maze[i][j] = Cell();
            maze[i][j].setSize(sf::Vector2f(w, h));
            maze[i][j].setPosition(sf::Vector2f(++j * w, ++i * h));
            --j;
            --i;
            // if (i % 2 == 0)
            // {
            //     maze[i][j].setWall(0, true);
            //     maze[i][j].setWall(1, true);
            // }
        }

    mazeBorder.setPosition(maze[0][0].getPosition());
}

void GameScreen::handleInput()
{
    sf::Event event;

    while (game->window->pollEvent(event))
    {
        // window closes if close button pressed
        if (event.type == sf::Event::Closed)
            game->window->close();

        if (event.type == sf::Event::MouseButtonPressed)
        {
            for (int i = 0; i < maze.size(); i++)
                for (int j = 0; j < maze[i].size(); j++)
                    maze[i][j].handleInput(event);
            
            if (event.mouseButton.button == sf::Mouse::Right)
            {

                // std::cout << "Input: " << std::endl;
                // int i, j;
                // std::cin >> i >> j;
                // char letter;
                // std::cin >> letter;
                // if (letter == 'v')
                //     maze[i][j].setWall(1, true);
                // else if (letter == 'h')
                //     maze[i][j].setWall(0, true);

                // std::cout << "Done" << std::endl;
            }
        }
    }
}

void GameScreen::update(const float dt)
{
    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].update(game->window);
}

void GameScreen::draw()
{
    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].renderCells(game->window);

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].renderLines(game->window);

    game->window->draw(mazeBorder);
}
