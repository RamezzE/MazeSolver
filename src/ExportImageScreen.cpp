#include "ExportImageScreen.hpp"

ExportImageScreen::ExportImageScreen(Game *myGame, Maze *maze)
{
    game = myGame;
    this->maze = maze;
    bgImg.loadFromFile(BACKGROUND_PATH);
    background.setTexture(&bgImg);

    gameScreen = dynamic_cast<GameScreen *>(game->getPreviousScreen());

    imagesFolder = "Exported Maze Images/";

    font.loadFromFile(FONT_PATH);

    for (int i = 0; i < 3; i++)
        textboxes.push_back(TextBox(font));

    for (int i = 0; i < 2; i++)
    {
        labels.push_back(sf::Text());
        labels[i].setFont(font);
    }

    labels[0].setString("File Name: ");
    labels[1].setString("Image dimensions: ");

    for (int i = 0; i < textboxes.size(); i++)
    {
        textboxes[i].setBackgroundColor(sf::Color(12, 13, 23, 255));
        textboxes[i].setBorder(3, sf::Color(12, 13, 23, 255), sf::Color(173, 172, 173), sf::Color::White);
    }

    textboxes[0].setTextLimit(100);
    textboxes[0].allowAlphaNumeric();

    for (int i = 1; i < 3; i++)
    {
        textboxes[i].setTextLimit(4);
        textboxes[i].allowNumberOnly();
        textboxes[i].setString("2000");
    }

    for (int i = 0; i < 4; i++)
    {
        myButtons.push_back(Button());
        myButtons[i].setFont(font);

        myButtons[i].setBorder(sf::Color(12, 13, 23, 255), 2);
        myButtons[i].setBackgroundColor(sf::Color(12, 13, 23, 255));
    }
    myButtons[2].setBackgroundColor(sf::Color::White);
    myButtons[3].setBackgroundColor(sf::Color::Black);

    X.setFont(font);
    X.setString("X");

    imagePreviewLabel.setString("Maze Preview: ");
    imagePreviewLabel.setFont(font);

    myButtons[0].setText("Export", sf::Color::Magenta);
    myButtons[1].setText("Back", sf::Color::Magenta);
    myButtons[2].setText("Toggle Color", sf::Color::Black);
    myButtons[3].setText("Open Folder", sf::Color::White);

    init();
}

void ExportImageScreen::init()
{
    background.setPosition(0, 0);
    background.setSize(sf::Vector2f(game->width, game->height));

    imagePreviewLabel.setCharacterSize(game->height / 20);
    imagePreviewLabel.setPosition(sf::Vector2f(game->width / 2.0f - imagePreviewLabel.getGlobalBounds().width / 2.0f, game->height / 8.0f));

    mazePreviewSize = sf::Vector2f(game->width / 2.0f, game->height / 2.0f);

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].setCharacterSize(game->height / 20);

    for (int i = 0; i < labels.size(); i++)
        labels[i].setCharacterSize(game->height / 30);

    X.setCharacterSize(game->height / 30);

    myButtons[0].setPosition(sf::Vector2f(game->width / 2.0f, game->height / 1.2f));

    myButtons[1].setPosition(sf::Vector2f(game->width * 0.05f, game->height * 0.05f));

    for (int i = 0; i < textboxes.size(); i++)
    {
        textboxes[i].setTextFormat(sf::Color::White, game->height / 20);
        textboxes[i].setSize(sf::Vector2f(game->width * 0.3, textboxes[i].getCharacterSize() * 1.2));
    }

    for (int i = 1; i < 3; i++)
        textboxes[i].setSize(sf::Vector2f(game->width * 0.07, textboxes[0].getCharacterSize() * 1.2));

    labels[0].setPosition(sf::Vector2f(game->width * 0.025, game->height * 0.2));
    textboxes[0].setPosition(sf::Vector2f(game->width * 0.025f, game->height * 0.25f));

    labels[1].setPosition(sf::Vector2f(game->width * 0.025, game->height * 0.35));
    textboxes[1].setPosition(sf::Vector2f(game->width * 0.025f, game->height * 0.4f));
    textboxes[2].setPosition(sf::Vector2f(textboxes[1].getPosition().x + textboxes[1].getGlobalBounds().width * 1.25, textboxes[1].getPosition().y));

    X.setOrigin(X.getLocalBounds().left, X.getLocalBounds().top + X.getLocalBounds().height / 2.0f);
    sf::Vector2f pos(textboxes[1].getPosition().x + textboxes[1].getGlobalBounds().width * 1, (textboxes[1].getPosition().y + (textboxes[1].getPosition().y + textboxes[1].getGlobalBounds().height)) / 2);
    X.setPosition(pos);

    myButtons[2].setPosition(sf::Vector2f(game->width / 1.5 + myButtons[2].getGlobalBounds().width / 2, textboxes[0].getPosition().y + myButtons[2].getGlobalBounds().height / 2));
    myButtons[3].setPosition(sf::Vector2f(textboxes[0].getPosition().x + myButtons[3].getGlobalBounds().width / 2, myButtons[0].getPosition().y));
}

void ExportImageScreen::handleInput()
{
    sf::Event event;
    while (game->window->pollEvent(event))
    {
        for (int i = 0; i < myButtons.size(); i++)
            myButtons[i].handleInput(event);

        for (int i = 0; i < textboxes.size(); i++)
            textboxes[i].handleInput(event);

        if (event.type == sf::Event::Closed)
        {
            game->window->close();
            return;
        }

        if (event.type == sf::Event::Resized)
        {
            checkResize(event);
            return;
        }
    }
}

void ExportImageScreen::update(const float dt)
{
    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].update(game->window);

    for (int i = 0; i < textboxes.size(); i++)
        textboxes[i].update(game->window);

    previewMaze();

    if (myButtons[0].isDoAction())
    {
        exportMazeToPNG();
        myButtons[0].didAction();
    }
    else if (myButtons[1].isDoAction())
    {
        maze->pause = false;
        dynamic_cast<GameScreen *>(game->getPreviousScreen())->init();
        game->previousScreen();
        myButtons[1].didAction();
    }
    else if (myButtons[2].isDoAction())
    {
        if (myButtons[2].getTextColor() == sf::Color::Magenta)
        {
            myButtons[2].setText("Toggle Color", sf::Color(12, 13, 23, 255));
            myButtons[2].setBackgroundColor(sf::Color::Magenta);
        }
        else if (myButtons[2].getTextColor() == sf::Color(12, 13, 23, 255))
        {
            myButtons[2].setText("Toggle Color", sf::Color::Black);
            myButtons[2].setBackgroundColor(sf::Color::White);
        }
        else if (myButtons[2].getTextColor() == sf::Color::Black)
        {
            myButtons[2].setText("Toggle Color", sf::Color::White);
            myButtons[2].setBackgroundColor(sf::Color::Black);
        }
        else if (myButtons[2].getTextColor() == sf::Color::White)
        {
            myButtons[2].setText("Toggle Color", sf::Color::Magenta);
            myButtons[2].setBackgroundColor(sf::Color(12, 13, 23, 255));
        }

        myButtons[2].didAction();
    }
    else if (myButtons[3].isDoAction())
    {

        // open folder in current directory. folder name is variable
        std::string command = "cd " + imagesFolder + " && explorer .";
        system(command.c_str());

        myButtons[3].didAction();
    }
}

void ExportImageScreen::draw()
{
    game->window->draw(background);
    game->window->draw(mazePreview);
    game->window->draw(imagePreviewLabel);
    game->window->draw(X);

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].render(game->window);

    for (int i = 0; i < textboxes.size(); i++)
        textboxes[i].draw(game->window);

    for (int i = 0; i < labels.size(); i++)
        game->window->draw(labels[i]);
}

void ExportImageScreen::previewMaze()
{
    Maze tempMaze = *maze;

    sf::Vector2f size(game->height / 2, game->height / 2);

    setPreviewColor(tempMaze);

    tempMaze.resize(size);
    tempMaze.setPosition(sf::Vector2f(0, 0));
    tempMaze.setWallThicknessFactor(tempMaze.getWallThicknessFactor());

    mazeRenderTexture.create(size.x, size.y);
    mazeRenderTexture.clear(sf::Color::White);

    tempMaze.render(&mazeRenderTexture);

    mazeRenderTexture.display();

    mazePreview = sf::Sprite();

    mazePreview.setTexture(mazeRenderTexture.getTexture());
    sf::FloatRect temp = mazePreview.getLocalBounds();
    mazePreview.setOrigin(temp.left + temp.width / 2.0f, temp.top + temp.height / 2.0f);

    mazePreview.setPosition(sf::Vector2f(game->width / 2.0f, game->height / 2.0f));
}

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos)
        return "";

    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

void ExportImageScreen::exportMazeToPNG()
{
    Maze tempMaze = *maze;

    setPreviewColor(tempMaze);

    int width = std::stoi(textboxes[1].getString());
    int height = std::stoi(textboxes[2].getString());

    if (trim(textboxes[1].getString()).length() == 0 && trim(textboxes[2].getString()).length() == 0)
        width = height = 2000;
    else
    {
        if (width < 1)
            width = 1;
        if (height < 1)
            height = 1;
    }

    tempMaze.resize(sf::Vector2f(width, height));
    tempMaze.setPosition(sf::Vector2f(0, 0));
    tempMaze.setWallThicknessFactor(tempMaze.getWallThicknessFactor());

    sf::RenderTexture texture;
    texture.create(width, height);
    texture.clear(sf::Color::White);

    tempMaze.render(&texture);
    texture.display();

    sf::Image image = texture.getTexture().copyToImage();
    // get date now
    time_t now = time(0);

    // create directory if it doesnt exist
    std::string dir = imagesFolder;
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

    std::string fileName = trim(textboxes[0].getString());
    if (fileName.length() > 0)
        dir = imagesFolder + fileName;

    // save image
    image.saveToFile(dir + ".png");

    // open image
    std::string cmd = "\"" + dir + ".png\"";
    system(cmd.c_str());
}

void ExportImageScreen::setPreviewColor(Maze &tempMaze)
{
    if (myButtons[2].getTextColor() == sf::Color::White)
    {
        tempMaze.setColors(sf::Color::Black, sf::Color::White, sf::Color::White);
    }
    else if (myButtons[2].getTextColor() == sf::Color::Black)
    {
        tempMaze.setColors(sf::Color::White, sf::Color::Black, sf::Color::Black);
    }
    else if (myButtons[2].getTextColor() == sf::Color::Magenta)
    {
        tempMaze.setColors(sf::Color(12, 13, 23, 255), sf::Color::Magenta, sf::Color(21, 23, 44));
    }
    else if (myButtons[2].getTextColor() == sf::Color(12, 13, 23, 255))
    {
        tempMaze.setColors(sf::Color::Magenta, sf::Color(12, 13, 23, 255), sf::Color(21, 23, 44));
    }
}

void ExportImageScreen::checkResize(sf::Event event)
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