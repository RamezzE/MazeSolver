#include "GameScreen.hpp"

int N = 50;

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
    maze->setColors(sf::Color(12, 13, 23, 255), sf::Color(21, 23, 44), sf::Color(21, 23, 44));

    bgImg.loadFromFile(BACKGROUND_PATH);

    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(game->width, game->height));
    background.setTexture(&bgImg);

    // background.setFillColor(sf::Color(31, 34, 61));

    font.loadFromFile(FONT_PATH);

    for (int i = 0; i < 3; i++)
        myButtons.push_back(Button());

    myButtons[0].setText("Resize Maze", sf::Color::Magenta);
    myButtons[1].setText("Generate Maze", sf::Color::Magenta);
    myButtons[2].setText("Solve Maze", sf::Color::Magenta);

    for (int i = 1; i <= myButtons.size(); i++)
    {
        myButtons[i - 1].setFont(font);
        myButtons[i - 1].setCharacterSize(40);

        sf::FloatRect temp = myButtons[i - 1].getGlobalBounds();
        myButtons[i - 1].setOrigin(sf::Vector2f(temp.left + temp.width / 2, temp.top + temp.height / 2));
        myButtons[i - 1].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 2, game->height * 4 / 8 + 100 * i));
        myButtons[i - 1].setBorder(sf::Color(12, 13, 23, 255), 2);
        myButtons[i - 1].setBackgroundColor(sf::Color(12, 13, 23, 255));
    }

    textBoxes.push_back(TextBox(font));

    textBoxes[0].setString(std::to_string(N));
    textBoxes[0].setTextLimit(2);
    textBoxes[0].allowNumberOnly();

    textBoxes[0].setTextFormat(sf::Color::Magenta, 40);
    textBoxes[0].setBackgroundColor(sf::Color(12, 13, 23, 255));
    textBoxes[0].setBorder(3, sf::Color(12, 13, 23, 255), sf::Color(173, 172, 173), sf::Color::Magenta);

    sf::FloatRect temp = textBoxes[0].getGlobalBounds();
    textBoxes[0].setOrigin(sf::Vector2f(temp.left + temp.width / 2, temp.top + temp.height / 2));
    textBoxes[0].setPosition(sf::Vector2f(textBoxes[0].getGlobalBounds().width / 4 + (pos.x + size.x) + (game->width - (pos.x + size.x)) / 2, game->height * 4 / 8));
    textBoxes[0].setSize(sf::Vector2f(textBoxes[0].getCharacterSize() * 1.2, textBoxes[0].getCharacterSize()));
}

void GameScreen::handleInput()
{
    sf::Event event;

    while (game->window->pollEvent(event))
    {
        for (int i = 0; i < textBoxes.size(); i++)
            textBoxes[i].handleInput(event);

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
                if (textBoxes[0].isSelected())
                {
                    textBoxes[0].setSelected(false);
                    resizeMaze = true;
                    return;
                }
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
        myButtons[i].update(game->window);

    for (int i = 0; i < textBoxes.size(); i++)
        textBoxes[i].update(game->window);

    if (myButtons[0].isDoAction() || resizeMaze)
    {
        N = stoi(textBoxes[0].getString());
        maze->resize(N, N);
        myButtons[0].didAction();
        resizeMaze = false;
    }
    else if (myButtons[1].isDoAction())
    {
        myThreads.push_back(std::thread{&Maze::generateMaze, std::ref(maze)});
        myThreads.back().detach();
        myButtons[1].didAction();
    }
    else if (myButtons[2].isDoAction())
    {
        myThreads.push_back(std::thread{&Maze::solveMaze, std::ref(maze), 0, 0, N - 1, N - 1});
        myThreads.back().detach();
        myButtons[2].didAction();
    }
}

void GameScreen::draw()
{
    game->window->draw(background);

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].render(game->window);

    for (int i = 0; i < textBoxes.size(); i++)
        textBoxes[i].draw(game->window);

    maze->render(game->window);
}
