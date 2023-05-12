#include "GameScreen.hpp"

GameScreen::GameScreen(Game *myGame)
{
    this->game = myGame;
    init();
}

void GameScreen::init()
{
    const int N = 20;
    maze.resize(N);

    for (int i = 0; i < N; i++)
        maze[i].resize(N);

    int w, h;
    w = game->width*0.9 / N;
    h = game->height*0.9 / N;

    mazeBorder.setOutlineColor(sf::Color(21, 23, 44));
    mazeBorder.setOutlineThickness(10);
    mazeBorder.setSize(sf::Vector2f(w * N, h * N));
    mazeBorder.setFillColor(sf::Color::Transparent);

    for (int i = 0; i < N; i++)
        for (int j = 0; j < maze[i].size(); j++)
        {
            maze[i][j] = Cell();
            maze[i][j].setSize(sf::Vector2f(w, h));
            maze[i][j].setPosition(sf::Vector2f((j + 1) * w, (i + 1) * h));

            if (i % 2 == 0)
            {
                maze[i][j].setWall(0, true);
                maze[i][j].setWall(1, true);
            }
        }

    mazeBorder.setPosition(maze[0][0].getPosition());

    player.setFillColor(sf::Color::Magenta);
    player.setSize(sf::Vector2f(w / 2, h / 2));

    player.setPosition(maze[0][0].getGlobalBounds().left + maze[0][0].getGlobalBounds().width / 4, maze[0][0].getGlobalBounds().top + maze[0][0].getGlobalBounds().height / 4);

    footprints.resize(N);
    for (int i = 0; i < N; i++)
        footprints[i].resize(N);

    for (int i = 0;i<footprints.size();i++) {
        for (int j = 0;j<footprints.size();j++) {
            footprints[i][j] = sf::RectangleShape(player);
            footprints[i][j].setFillColor(sf::Color::Transparent);
            footprints[i][j].setPosition(maze[i][j].getGlobalBounds().left + maze[i][j].getGlobalBounds().width / 4, maze[i][j].getGlobalBounds().top + maze[i][j].getGlobalBounds().height / 4);
        }
    }
}

void GameScreen::handleInput()
{
    sf::Event event;

    while (game->window->pollEvent(event))
    {
        // window closes if close button pressed
        if (event.type == sf::Event::Closed) {
            game->window->close();
            for (int i = 0;i<myThreads.size();i++)
                myThreads[i].~thread();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Enter) {
                myThreads.push_back(std::thread{&Maze::solveMaze, std::ref(maze), 0, 0, maze.size()-1,maze[0].size()-1, std::ref(player), std::ref(footprints)});
                myThreads.back().detach();
            }
        }

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
    game->window->draw(mazeBorder);

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++) {
            maze[i][j].renderCells(game->window);
            game->window->draw(footprints[i][j]);
        }

    for (int i = 0; i < maze.size(); i++)
        for (int j = 0; j < maze[i].size(); j++)
            maze[i][j].renderLines(game->window);

    game->window->draw(player);
}
