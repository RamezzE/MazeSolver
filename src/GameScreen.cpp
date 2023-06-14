#include "GameScreen.hpp"

int N = 10;

GameScreen::GameScreen(Game *myGame)
{
    this->game = myGame;

    for (int i = 0; i < 2; i++)
    {
        sliders.push_back(new Slider());
        sliders[i]->setColors(sf::Color(21, 23, 44), sf::Color::Magenta);
    }

    sliders[0]->create(0, 11);
    sliders[0]->setSliderValue(1);

    sliders[1]->create(1, 4);

    font.loadFromFile(FONT_PATH);

    for (int i = 0; i < 6; i++)
    {
        myButtons.push_back(Button());
        myButtons[i].setFont(font);

        myButtons[i].setBorder(sf::Color(12, 13, 23, 255), 2);
        myButtons[i].setBackgroundColor(sf::Color(12, 13, 23, 255));
    }

    textBoxes.push_back(TextBox(font));

    textBoxes[0].setString(std::to_string(N));
    textBoxes[0].setTextLimit(2);
    textBoxes[0].allowNumberOnly();

    textBoxes[0].setTextFormat(sf::Color::Magenta, 40);
    textBoxes[0].setBackgroundColor(sf::Color(12, 13, 23, 255));
    textBoxes[0].setBorder(3, sf::Color(12, 13, 23, 255), sf::Color(173, 172, 173), sf::Color::Magenta);

    bgImg.loadFromFile(BACKGROUND_PATH);
    background.setTexture(&bgImg);

    sf::Vector2f size(game->height / 1.2, game->height / 1.2);

    sf::Vector2f pos(game->width * 0.025, game->height / 2 - size.y / 2);
    maze = new Maze(N, N, size, pos);
    maze->setColors(sf::Color(12, 13, 23, 255), sf::Color(21, 23, 44), sf::Color(21, 23, 44));

    game->window->setPosition(sf::Vector2i(0, 0));

    myButtons[0].setText("  Resize  \n   Maze  ", sf::Color::Magenta);
    myButtons[1].setText("Generate Maze", sf::Color::Magenta);
    myButtons[2].setText("Solve Maze", sf::Color::Magenta);
    myButtons[3].setText("  Choose\nStart/End", sf::Color::Magenta);
    myButtons[4].setText("  Export\nas Image", sf::Color::Magenta);
    myButtons[5].setText("Shortest Path", sf::Color::Magenta);

    for (int i = 0; i < 2; i++)
    {
        labels.push_back(sf::Text());
        labels[i].setFont(font);
        labels[i].setFillColor(sf::Color::Magenta);
        labels[i].setOutlineColor(sf::Color(12, 13, 23, 255));
        labels[i].setOutlineThickness(1);
    }

    labels[0].setString("Speed: ");

    labels[1].setString("Thickness: ");

    exportMaze = maximized = false;
    exportImageScreen = new ExportImageScreen(game, maze);

    for (int i = 0; i < 2; i++)
    {
        notesBackground.push_back(sf::RectangleShape());
        notesBackground[i].setFillColor(sf::Color(12, 13, 23, 255));
    }

    for (int i = 0; i < 2; i++)
    {
        notesText.push_back(sf::Text());
        notesText[i].setFont(font);
        notesText[i].setFillColor(sf::Color(255, 255, 255, 128));
    }

    notesText[0].setString("N.B.\nOn any cell:\nLeft click: toggle top wall.\nRight click: toggle right wall.\n\nSpeed 0 -> pause simulation.\nMax Speed -> Instant effect.");
    notesText[1].setString("N.B.\nGenerate Maze: generates a\nmaze with only 1 solution\ninitially.\n\nSolve Maze: stops at 1st\nsolution found.");

    init();
}

void GameScreen::init()
{
    sf::Vector2f size(game->height / 1.2, game->height / 1.2);
    sf::Vector2f pos(game->width * 0.05, game->height * 0.05);

    maze->resize(size);
    maze->setPosition(sf::Vector2f(game->width * 0.025, game->height / 2 - size.y / 2));

    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(game->width, game->height));

    for (int i = 0; i < 6; i++)
        myButtons[i].setCharacterSize(game->height / 20);

    for (int i = 0; i < 2; i++)
        labels[i].setCharacterSize(game->height / 20);

    for (int i = 0; i < 2; i++)
        notesText[i].setCharacterSize(game->height / 30);

    textBoxes[0].setTextFormat(sf::Color::Magenta, game->height / 20);
    textBoxes[0].setSize(sf::Vector2f(textBoxes[0].getCharacterSize() * 1.3, textBoxes[0].getCharacterSize()));

    myButtons[1].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 4, game->height * 4 / 10 + myButtons[0].getLocalBounds().height * (1 - 1) * 0.7));
    myButtons[5].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 4, game->height * 4 / 10 + myButtons[0].getLocalBounds().height * (2 - 1) * 0.7));

    myButtons[2].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 4, game->height * 4 / 10 + myButtons[0].getLocalBounds().height * (2) * 0.7));

    myButtons[0].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 1.2, game->height * 3.2 / 10));
    textBoxes[0].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 1.2 - textBoxes[0].getGlobalBounds().width / 2, game->height * 1.95 / 10));
    myButtons[3].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 1.2, game->height * 4.75 / 10));
    myButtons[4].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 1.2, game->height * 1 / 10));

    for (int i = 0; i < sliders.size(); i++)
    {
        sliders[i]->setAxisSize(sf::Vector2f(game->width / 5, game->height / 80));
        sliders[i]->setHandleSize(sf::Vector2f(game->height / 30, game->height / 20));
        sliders[i]->setCharacterSize(game->height / 50);
    }

    sliders[0]->setPosition(sf::Vector2f(textBoxes[0].getGlobalBounds().width / 4 + (pos.x + size.x) + (game->width - (pos.x + size.x)) / 4 - sliders[0]->getGlobalBounds().width / 2, pos.y * 2.8));

    sliders[1]->setPosition(sf::Vector2f(textBoxes[0].getGlobalBounds().width / 4 + (pos.x + size.x) + (game->width - (pos.x + size.x)) / 4 - sliders[0]->getGlobalBounds().width / 2, pos.y * 6));

    labels[0].setPosition(sf::Vector2f(sliders[0]->getGlobalBounds().left, pos.y / 2));
    labels[1].setPosition(sf::Vector2f(sliders[1]->getGlobalBounds().left, pos.y * 3.8));

    for (int i = 0; i < 2; i++)
    {
        notesBackground[i].setPosition(sf::Vector2f(myButtons[1].getPosition().x - myButtons[1].getGlobalBounds().width / 1.5, game->height * 6.2 / 10));
        notesBackground[i].setSize(sf::Vector2f((myButtons[0].getGlobalBounds().left - labels[0].getGlobalBounds().left) * 0.74, game->height / 3.37));
        notesText[i].setPosition(notesBackground[0].getPosition());
    }

    notesText[0].move(notesText[1].getCharacterSize() / 3, notesText[1].getCharacterSize() / 6);
    notesText[1].move(notesText[0].getGlobalBounds().width * 1.16, notesText[1].getCharacterSize() / 6);
    notesBackground[1].move(notesText[0].getGlobalBounds().width * 1.13, 0);

    if (maximized)
    {
        notesText[1].move(19.5, 0);
        notesBackground[1].move(19.5, 0);

        for (int i = 0; i < 2; i++)
            notesBackground[i].setSize(sf::Vector2f(notesBackground[i].getSize().x * 0.9, notesBackground[i].getSize().y));
    }
}

void GameScreen::handleInput()
{
    sf::Event event;

    while (game->window->pollEvent(event))
    {
        for (int i = 0; i < textBoxes.size(); i++)
            textBoxes[i].handleInput(event);

        for (int i = 0; i < sliders.size(); i++)
            sliders[i]->handleInput(event);

        for (int i = 0; i < myButtons.size(); i++)
            myButtons[i].handleInput(event);

        maze->handleInput(event);

        if (event.type == sf::Event::Resized)
        {
            checkResize(event);
            break;
        }
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
        }
    }
}

void GameScreen::update(const float dt)
{

    maze->pause = false;
    switch ((int)sliders[0]->getSliderValue())
    {
    case 0:
        maze->pause = true;
        break;
    case 1:
        maze->setSpeedFactor(1);
        break;
    case 2:
        maze->setSpeedFactor(4);
        break;
    case 3:
        maze->setSpeedFactor(8);
        break;
    case 4:
        maze->setSpeedFactor(16);
        break;
    case 5:
        maze->setSpeedFactor(32);
        break;
    case 6:
        maze->setSpeedFactor(64);
        break;
    case 7:
        maze->setSpeedFactor(128);
        break;
    case 8:
        maze->setSpeedFactor(256);
        break;
    case 9:
        maze->setSpeedFactor(312);
        break;
    case 10:
        maze->setSpeedFactor(500);
        break;
    case 11:
        maze->setSpeedFactor(2048);
        break;
    default:
        maze->setSpeedFactor(0);
        break;
    }

    switch ((int)sliders[1]->getSliderValue())
    {
    case 1:
        maze->setWallThicknessFactor(1);
        break;
    case 2:
        maze->setWallThicknessFactor(2);
        break;
    case 3:
        maze->setWallThicknessFactor(4);
        break;
    case 4:
        maze->setWallThicknessFactor(8);
        break;
    }

    for (int i = 0; i < textBoxes.size(); i++)
        textBoxes[i].update(game->window);

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].update(game->window);

    for (int i = 0; i < sliders.size(); i++)
        sliders[i]->update(game->window);

    maze->update(game->window);

    if (maze->threadRunning || maze->choosingStartOrEnd)
    {
        for (int i = 0; i < myButtons.size(); i++)
            myButtons[i].setEnabled(false);

        myButtons[4].setEnabled(true);
        resizeMaze = false;
        // return;
    }
    else
    {
        for (int i = 0; i < myButtons.size(); i++)
            myButtons[i].setEnabled(true);

        if (!maze->mazeGenerated)
        {
            myButtons[2].setEnabled(false);
            myButtons[3].setEnabled(false);
            myButtons[5].setEnabled(false);
        }
    }

    if (myButtons[0].isDoAction() || resizeMaze)
    {
        N = stoi(textBoxes[0].getString());
        maze->resizeGrid(N, N);
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
        myThreads.push_back(std::thread{&Maze::solveMaze, std::ref(maze)});
        myThreads.back().detach();
        myButtons[2].didAction();
    }
    else if (myButtons[3].isDoAction())
    {
        maze->choosingStartOrEnd = true;
        myButtons[3].didAction();
    }
    else if (myButtons[4].isDoAction())
    {
        maze->pause = true;
        exportImageScreen->init();
        game->changeScreen(exportImageScreen);
        myButtons[4].didAction();
    }
    else if (myButtons[5].isDoAction())
    {
        myThreads.push_back(std::thread{&Maze::findShortestPath, std::ref(maze)});
        myThreads.back().detach();
        myButtons[5].didAction();
    }
}

void GameScreen::draw()
{
    game->window->draw(background);

    for (int i = 0; i < notesBackground.size(); i++)
        game->window->draw(notesBackground[i]);

    for (int i = 0; i < notesText.size(); i++)
        game->window->draw(notesText[i]);

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].render(game->window);

    for (int i = 0; i < textBoxes.size(); i++)
        textBoxes[i].draw(game->window);

    maze->render(game->window);

    for (int i = 0; i < 2; i++)
        sliders[i]->render(game->window);

    for (int i = 0; i < labels.size(); i++)
        game->window->draw(labels[i]);
}

void GameScreen::checkResize(sf::Event event)
{
    sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);

    int h = 0, w = 0;

    if (sf::VideoMode::getDesktopMode().width == event.size.width || sf::VideoMode::getDesktopMode().height == event.size.height)
    {
        h = sf::VideoMode::getDesktopMode().height;
        w = sf::VideoMode::getDesktopMode().width;
    }
    else if (event.size.height != game->height)
    {
        while (h <= event.size.height)
            h += 9;

        h -= 9;
        w = h * 16 / 9;
        game->window->setSize(sf::Vector2u(w, h));
    }
    else if (event.size.width != game->width)
    {
        while (w <= event.size.width)
            w += 16;

        w -= 16;
        h = w * 9 / 16;
        game->window->setSize(sf::Vector2u(w, h));
    }
    else if (event.size.height == game->height && event.size.width == game->width)
        return;

    visibleArea = sf::FloatRect(0, 0, w, h);

    if (event.size.height == sf::VideoMode::getDesktopMode().height || event.size.width == sf::VideoMode::getDesktopMode().width)
    {
        if (!event.size.height == sf::VideoMode::getDesktopMode().height || !event.size.width == sf::VideoMode::getDesktopMode().width)
            game->window->setPosition(sf::Vector2i(0, 0));

        game->window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
        maximized = true;
    }
    else
    {
        maximized = false;
        game->window->setView(sf::View(visibleArea));
    }

    game->width = game->window->getSize().x;
    game->height = game->window->getSize().y;

    init();
}