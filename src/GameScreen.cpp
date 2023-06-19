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

    for (int i = 0; i < 1; i++)
    {
        textBoxes.push_back(TextBox(font));
        textBoxes[0].allowNumberOnly();
        textBoxes[0].setTextFormat(sf::Color::Magenta, 40);
        textBoxes[0].setBackgroundColor(sf::Color(12, 13, 23, 255));
        textBoxes[0].setBorder(3, sf::Color(12, 13, 23, 255), sf::Color(173, 172, 173), sf::Color::Magenta);
    }

    textBoxes[0].setTextLimit(2);
    textBoxes[0].setString(std::to_string(N));

    for (int i = 0; i < 1; i++)
    {
        checkboxes.push_back(CheckBox());
        checkboxes[i].setBackgroundColor(sf::Color(12, 13, 23, 255));
        checkboxes[i].setCheckedColor(sf::Color::Magenta);
    }

    bgImg.loadFromFile(BACKGROUND_PATH);
    background.setTexture(&bgImg);

    sf::Vector2f size(game->height / 1.2, game->height / 1.2);

    sf::Vector2f pos(game->width * 0.025, game->height / 2 - size.y / 2);
    maze = new Maze(N, N, size, pos);
    maze->setColors(sf::Color(12, 13, 23, 255), sf::Color(21, 23, 44), sf::Color(21, 23, 44));

    game->window->setPosition(sf::Vector2i(0, 0));

    myButtons[0].setText("Generate Maze", sf::Color::Magenta);
    myButtons[1].setText("Shortest Path", sf::Color::Magenta);
    myButtons[2].setText("Solve Maze", sf::Color::Magenta);
    myButtons[3].setText("  Export\nas Image", sf::Color::Magenta);
    myButtons[4].setText("  Resize  \n   Maze  ", sf::Color::Magenta);
    myButtons[5].setText("  Choose\nStart/End", sf::Color::Magenta);

    for (int i = 0; i < 3; i++)
    {
        labels.push_back(sf::Text());
        labels[i].setFont(font);
        labels[i].setFillColor(sf::Color::Magenta);
        labels[i].setOutlineColor(sf::Color(12, 13, 23, 255));
        labels[i].setOutlineThickness(1);
    }

    labels[0].setString("Speed: ");
    labels[1].setString("Thickness: ");
    labels[2].setString("Toggle Edit\n     Mode");

    exportMaze = maximized = false;
    exportImageScreen = new ExportImageScreen(game, maze);

    for (int i = 0; i < 2; i++)
    {
        notesBackground.push_back(sf::RectangleShape());
        notesBackground[i].setFillColor(sf::Color(12, 13, 23, 255));
        notesBackground[i].setOutlineColor(sf::Color::Magenta);
    }

    for (int i = 0; i < 2; i++)
    {
        notesText.push_back(sf::Text());
        notesText[i].setFont(font);
        notesText[i].setFillColor(sf::Color(255, 255, 255, 128));
    }
    notesText[0].setString("N.B.\nGenerate Maze: generates a\nmaze with only 1 solution\ninitially\n\nSolve Maze: stops at 1st\nsolution found");
    notesText[1].setString("N.B.\nOn any cell in Edit Mode:\nLeft click: toggle top wall\nRight click: toggle right wall\n\nSpeed 0 -> pause simulation\nMax Speed -> Instant effect");

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
    labels[2].setCharacterSize(game->height / 30);

    for (int i = 0; i < 2; i++)
        notesText[i].setCharacterSize(game->height / 30);

    for (int i = 0; i < 3; i++)
        myButtons[i].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 4, game->height * 4 / 10 + myButtons[4].getLocalBounds().height * i * 0.7));

    textBoxes[0].setTextFormat(sf::Color::Magenta, game->height / 20);
    textBoxes[0].setSize(sf::Vector2f(textBoxes[0].getCharacterSize() * 1.3, textBoxes[0].getCharacterSize()));
    textBoxes[0].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 1.2 - textBoxes[0].getGlobalBounds().width / 2, game->height * 1.95 / 10));

    myButtons[3].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 1.2, game->height * 1 / 10));
    myButtons[4].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 1.2, game->height * 3.2 / 10));
    myButtons[5].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 1.2, game->height * 4.75 / 10));

    for (int i = 0; i < sliders.size(); i++)
    {
        sliders[i]->setAxisSize(sf::Vector2f(game->width / 5, game->height / 80));
        sliders[i]->setHandleSize(sf::Vector2f(game->height / 30, game->height / 20));
        sliders[i]->setCharacterSize(game->height / 50);
    }

    for (int i = 0; i < 2; i++)
        sliders[i]->setPosition(sf::Vector2f(textBoxes[0].getGlobalBounds().width / 4 + (pos.x + size.x) + (game->width - (pos.x + size.x)) / 4 - sliders[i]->getGlobalBounds().width / 2, pos.y * (i + 1) * 3));

    for (int i = 0; i < 2; i++)
        labels[i].setPosition(sf::Vector2f(sliders[i]->getGlobalBounds().left, sliders[i]->getGlobalBounds().top - labels[i].getGlobalBounds().height * 2.5));

    for (int i = 0; i < 2; i++)
    {
        notesBackground[i].setPosition(sf::Vector2f(myButtons[0].getPosition().x - myButtons[0].getGlobalBounds().width / 1.5, game->height * 6.2 / 10));
        notesBackground[i].setSize(sf::Vector2f((myButtons[3].getGlobalBounds().left - labels[0].getGlobalBounds().left) * 0.74, game->height / 3.37));
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

    for (int i = 0; i < checkboxes.size(); i++)
    {
        checkboxes[i].setSize(sf::Vector2f(game->height / 20, game->height / 20));
        checkboxes[i].setPosition(sf::Vector2f(myButtons[5].getPosition().x - myButtons[5].getGlobalBounds().width / 2 + checkboxes[i].getSize().x / 2, myButtons[5].getPosition().y + checkboxes[i].getSize().y * 2 * (i + 1)));
    }

    labels[2].setPosition(checkboxes[0].getPosition().x + checkboxes[0].getSize().x * 0.8, checkboxes[0].getPosition().y - checkboxes[0].getSize().y / 1.3);
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

        for (int i = 0; i < checkboxes.size(); i++)
            checkboxes[i].handleInput(event);

        maze->handleInput(event, game->window);

        if (event.type == sf::Event::Resized)
        {
            checkResize(event);
            return;
        }

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

    for (int i = 0; i < checkboxes.size(); i++)
        checkboxes[i].update(game->window);

    maze->update(game->window);

    if (checkboxes[0].isChecked())
        maze->editMode = true;
    else
        maze->editMode = false;

    if (maze->threadRunning)
    {
        notesText[0].setFillColor(sf::Color::Magenta);
        notesBackground[0].setOutlineThickness(2);
    }
    else
    {
        notesText[0].setFillColor(sf::Color(255, 255, 255, 128));
        notesBackground[0].setOutlineThickness(0);
    }

    if (maze->editMode)
    {
        notesText[1].setFillColor(sf::Color::Magenta);
        notesBackground[1].setOutlineThickness(2);
    }
    else
    {
        notesText[1].setFillColor(sf::Color(255, 255, 255, 128));
        notesBackground[1].setOutlineThickness(0);
    }

    if (maze->threadRunning || maze->choosingStartOrEnd)
        checkboxes[0].setEnabled(false);
    else
        checkboxes[0].setEnabled(true);

    if (maze->threadRunning || maze->choosingStartOrEnd || maze->editMode)
    {
        for (int i = 0; i < myButtons.size(); i++)
            myButtons[i].setEnabled(false);
        resizeMaze = false;
        return;
    }

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].setEnabled(true);

    if (!maze->mazeGenerated)
    {
        myButtons[2].setEnabled(false);
        myButtons[3].setEnabled(false);
        myButtons[1].setEnabled(false);
    }

    if (myButtons[0].isDoAction())
    {
        myThreads.push_back(std::thread{&Maze::generateMaze, std::ref(maze)});
        myThreads.back().detach();
        myButtons[0].didAction();
    }
    else if (myButtons[1].isDoAction())
    {
        myThreads.push_back(std::thread{&Maze::findShortestPath, std::ref(maze)});
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
        maze->pause = true;
        exportImageScreen->init();
        game->changeScreen(exportImageScreen);
        myButtons[3].didAction();
    }
    else if (myButtons[4].isDoAction() || resizeMaze)
    {
        N = stoi(textBoxes[0].getString());
        maze->resizeGrid(N, N);
        myButtons[4].didAction();
        resizeMaze = false;
    }
    else if (myButtons[5].isDoAction())
    {
        maze->choosingStartOrEnd = true;
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

    for (int i = 0; i < sliders.size(); i++)
        sliders[i]->render(game->window);

    for (int i = 0; i < labels.size(); i++)
        game->window->draw(labels[i]);

    for (int i = 0; i < checkboxes.size(); i++)
        checkboxes[i].render(game->window);

    maze->render(game->window);
}

void GameScreen::checkResize(sf::Event event)
{
    if (event.size.height == game->height && event.size.width == game->width)
        return;

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