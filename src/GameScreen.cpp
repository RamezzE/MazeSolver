#include "GameScreen.hpp"

int N = 10;

GameScreen::GameScreen(Game *myGame)
{
    this->game = myGame;

    speedSlider.create(0, 11);

    font.loadFromFile(FONT_PATH);

    for (int i = 0; i < 5; i++)
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
    sf::Vector2f pos(game->width * 0.025, game->height * 0.025);
    maze = new Maze(N, N, size, pos);
    maze->setColors(sf::Color(12, 13, 23, 255), sf::Color(21, 23, 44), sf::Color(21, 23, 44));

    game->window->setPosition(sf::Vector2i(0, 0));

    myButtons[0].setText("Resize Maze", sf::Color::Magenta);
    myButtons[1].setText("Generate Maze", sf::Color::Magenta);
    myButtons[2].setText("Solve Maze", sf::Color::Magenta);
    myButtons[3].setText("Choose Start/End", sf::Color::Magenta);
    myButtons[4].setText("Export as Image", sf::Color::Magenta);

    speedSlider.setColors(sf::Color(21, 23, 44), sf::Color::Magenta);

    speedLabel.setString("Speed: ");
    speedLabel.setFont(font);
    speedLabel.setFillColor(sf::Color::Magenta);

    exportMaze = false;

    init();
}

void GameScreen::init()
{
    sf::Vector2f size(game->height / 1.2, game->height / 1.2);
    sf::Vector2f pos(game->width * 0.05, game->height * 0.05);

    maze->resize(size);

    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(game->width, game->height));

    for (int i = 0; i < 5; i++)
        myButtons[i].setCharacterSize(game->height / 20);

    speedLabel.setCharacterSize(game->height / 20);
    textBoxes[0].setTextFormat(sf::Color::Magenta, game->height / 20);
    textBoxes[0].setSize(sf::Vector2f(textBoxes[0].getCharacterSize() * 1.2, textBoxes[0].getCharacterSize()));

    for (int i = 1; i <= 3; i++)
    {
        sf::FloatRect temp = myButtons[i - 1].getGlobalBounds();
        myButtons[i - 1].setPosition(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 2, game->height * 4 / 8 + 2 * myButtons[0].getLocalBounds().height * i));
    }

    myButtons[3].setPosition(sf::Vector2f(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 2, game->height * 3 / 8)));

    myButtons[4].setPosition(sf::Vector2f(sf::Vector2f((pos.x + size.x) + (game->width - (pos.x + size.x)) / 2, game->height * 1 / 10)));

    textBoxes[0].setPosition(sf::Vector2f(myButtons[2].getGlobalBounds().left + myButtons[2].getGlobalBounds().width / 2 - textBoxes[0].getGlobalBounds().width / 2, game->height * 4 / 8));

    speedSlider.setAxisSize(sf::Vector2f(game->width / 5, game->height / 80));
    speedSlider.setHandleSize(sf::Vector2f(game->height / 30, game->height / 20));
    speedSlider.setPosition(sf::Vector2f(textBoxes[0].getGlobalBounds().width / 4 + (pos.x + size.x) + (game->width - (pos.x + size.x)) / 2 - speedSlider.getGlobalBounds().width / 2, game->height * 2.2 / 8));

    speedLabel.setPosition(sf::Vector2f(speedSlider.getGlobalBounds().left, game->height * 1.3 / 8));
}
void GameScreen::handleInput()
{
    sf::Event event;

    while (game->window->pollEvent(event))
    {
        for (int i = 0; i < textBoxes.size(); i++)
            textBoxes[i].handleInput(event);

        speedSlider.handleInput(event);

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

    switch ((int)speedSlider.getSliderValue())
    {
    case 0:
        maze->setSpeedFactor(1);
        break;
    case 1:
        maze->setSpeedFactor(4);
        break;
    case 2:
        maze->setSpeedFactor(8);
        break;
    case 3:
        maze->setSpeedFactor(16);
        break;
    case 4:
        maze->setSpeedFactor(32);
        break;
    case 5:
        maze->setSpeedFactor(64);
        break;
    case 6:
        maze->setSpeedFactor(128);
        break;
    case 7:
        maze->setSpeedFactor(256);
        break;
    case 8:
        maze->setSpeedFactor(312);
        break;
    case 9:
        maze->setSpeedFactor(500);
        break;
    case 10:
        maze->setSpeedFactor(2048);
        break;
    default:
        maze->setSpeedFactor(0);
        break;
    }

    for (int i = 0; i < textBoxes.size(); i++)
        textBoxes[i].update(game->window);

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].update(game->window);

    speedSlider.update(game->window);

    maze->update(game->window);

    if (maze->threadRunning || maze->choosingStartOrEnd)
    {
        for (int i = 0; i < myButtons.size(); i++)
            myButtons[i].setEnabled(false);

        myButtons[4].setEnabled(true);
        resizeMaze = false;
        return;
    }
    else
    {
        for (int i = 0; i < myButtons.size(); i++)
            myButtons[i].setEnabled(true);

        if (!maze->mazeGenerated)
        {
            myButtons[2].setEnabled(false);
            myButtons[3].setEnabled(false);
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
        exportMazeToPNG();
        myButtons[4].didAction();
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

    speedSlider.render(*(game->window));
    game->window->draw(speedLabel);
}

void GameScreen::exportMazeToPNG()
{
    Maze tempMaze = *maze;

    tempMaze.setColors(sf::Color::White, sf::Color::Black, sf::Color::Black);
    tempMaze.resize(sf::Vector2f(2000, 2000));
    tempMaze.setPosition(sf::Vector2f(0, 0));

    sf::RenderTexture texture;
    texture.create(2000, 2000);
    texture.clear(sf::Color::White);

    tempMaze.render(&texture);
    texture.display();

    sf::Image image = texture.getTexture().copyToImage();
    // get date now
    time_t now = time(0);

    // create directory if it doesnt exist
    std::string dir = "Exported Maze Images/";
    if (!fs::exists(dir))
        fs::create_directory(dir);

    // save file with current time
    dir += ctime(&now);
    char characterToRemove = ':';

    std::string result;
    for (char c : dir)
        if (c != characterToRemove && c != '\n')
            result += c;
    dir = result;

    // save image
    image.saveToFile(dir + ".png");

    // open image
    std::string cmd = "\"" + dir + ".png\"";
    system(cmd.c_str());
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
        game->window->setPosition(sf::Vector2i(0, 0));
        game->window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
    }
    else
        game->window->setView(sf::View(visibleArea));

    game->width = game->window->getSize().x;
    game->height = game->window->getSize().y;

    init();
}