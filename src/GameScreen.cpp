#include "GameScreen.hpp"

int N = 10;

GameScreen::GameScreen(Game *myGame)
{
    this->game = myGame;
    init();
}

void GameScreen::init()
{
    sf::Vector2f size(game->height / 1.1, game->height / 1.1);
    sf::Vector2f pos(game->width * 0.05, game->height * 0.05);

    maze = new Maze(N, N, size, pos);
    maze->setColors(sf::Color(12, 13, 23, 255), sf::Color(173, 172, 173), sf::Color(173, 172, 173));

    font.loadFromFile(FONT_PATH);

    for (int i = 0; i < 2; i++)
        myButtons.push_back(Button());

    myButtons[0].setText("Generate Maze", sf::Color::Magenta);
    myButtons[1].setText("Solve Maze", sf::Color::Magenta);

    for (int i = 0; i < myButtons.size(); i++)
    {
        myButtons[i].setFont(font);
        myButtons[i].setCharacterSize(40);

        sf::FloatRect temp = myButtons[i].getGlobalBounds();
        myButtons[i].setOrigin(sf::Vector2f(temp.left + temp.width / 2, temp.top + temp.height / 2));
        myButtons[i].setPosition(sf::Vector2f(game->width - myButtons[0].getGlobalBounds().width / 1.6, game->height * 3 / 4 + 100 * i));
        myButtons[i].setBorder(sf::Color(173, 172, 173), 0);
        myButtons[i].setBackgroundColor(sf::Color(12, 13, 23, 255));
    }
}

void GameScreen::handleInput()
{
    sf::Event event;

    while (game->window->pollEvent(event))
    {
        // window closes if close button pressed
        if (event.type == sf::Event::Closed)
        {
            game->window->close();
            for (int i = 0; i < myThreads.size(); i++)
                myThreads[i].~thread();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Num1:
                maze->setSpeedFactor(0);
                break;
            case sf::Keyboard::Num2:
                maze->setSpeedFactor(1);
                break;
            case sf::Keyboard::Num3:
                maze->setSpeedFactor(4);
                break;
            case sf::Keyboard::Num4:
                maze->setSpeedFactor(8);
                break;
            case sf::Keyboard::Num5:
                maze->setSpeedFactor(24);
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            maze->handleInput(event);
            for (int i = 0; i < myButtons.size(); i++)
                myButtons[i].handleInput(event);
            return;
        }
        if (event.type == sf::Event::MouseButtonReleased)
            for (int i = 0; i < myButtons.size(); i++)
                myButtons[i].handleInput(event);
    }
}

void GameScreen::update(const float dt)
{
    maze->update(game->window);
    for (int i = 0; i < myButtons.size(); i++)
    {
        myButtons[i].update(game->window);
    }

    if (myButtons[0].isDoAction())
    {
        myThreads.push_back(std::thread{&Maze::generateMaze, std::ref(maze)});
        myThreads.back().detach();
        myButtons[0].didAction();
    }
    else if (myButtons[1].isDoAction())
    {
        myThreads.push_back(std::thread{&Maze::solveMaze, std::ref(maze), 0, 0, N - 1, N - 1});
        myThreads.back().detach();
        myButtons[1].didAction();
    }
}

void GameScreen::draw()
{
    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(game->width, game->height));
    background.setFillColor(sf::Color::White);

    game->window->draw(background);
    maze->render(game->window);

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].render(game->window);
}
