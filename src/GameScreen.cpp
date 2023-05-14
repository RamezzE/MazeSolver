#include "GameScreen.hpp"

int N = 30;

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

    myButton.setFont(font);
    myButton.setText("Generate Maze", sf::Color::Magenta);
    myButton.setCharacterSize(30);
    sf::FloatRect temp = myButton.getGlobalBounds();
    myButton.setOrigin(sf::Vector2f(temp.left + temp.width / 2, temp.top + temp.height / 2));

    myButton.setPosition(sf::Vector2f(game->width - temp.width / 1.8, game->height * 3 / 4));
    myButton.setBorder(sf::Color(173, 172, 173), 0);
    myButton.setBackgroundColor(sf::Color(12, 13, 23, 255));
}

void GameScreen::handleInput()
{
    sf::Event event;

    while (game->window->pollEvent(event))
    {
        myButton.handleInput(event);
        // window closes if close button pressed
        if (event.type == sf::Event::Closed)
        {
            game->window->close();
            for (int i = 0; i < myThreads.size(); i++)
                myThreads[i].~thread();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Enter)
            {
                myThreads.push_back(std::thread{&Maze::solveMaze, std::ref(maze), 0, 0, N - 1, N - 1});
                myThreads.back().detach();
                return;
            }
            if (event.key.code == sf::Keyboard::R)
            {
                myThreads.push_back(std::thread{&Maze::generateMaze, std::ref(maze)});
                myThreads.back().detach();
                return;
            }

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
                maze->setSpeedFactor(12);
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
            maze->handleInput(event);
    }
}

void GameScreen::update(const float dt)
{
    maze->update(game->window);
    myButton.update(game->window);

    if (myButton.isDoAction())
    {
        myThreads.push_back(std::thread{&Maze::generateMaze, std::ref(maze)});
        myThreads.back().detach();
        myButton.didAction();
    }
}

void GameScreen::draw()
{
    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(game->width, game->height));
    background.setFillColor(sf::Color::White);

    game->window->draw(background);
    maze->render(game->window);
    myButton.render(game->window);
}
