#include "ExportImageScreen.hpp"

ExportImageScreen::ExportImageScreen(Game *myGame, Maze *maze)
{
    game = myGame;
    this->maze = maze;
    bgImg.loadFromFile(BACKGROUND_PATH);
    background.setTexture(&bgImg);

    font.loadFromFile(FONT_PATH);

    for (int i = 0; i < 1; i++)
        textboxes.push_back(TextBox(font));

    // time of now

    time_t now = time(0);

    std::string dir = "Exported Maze Images/";

    // save file with current time
    char characterToRemove = ':';

    std::string fileName = ctime(&now);

    std::string result;
    for (char c : fileName)
        if (c != characterToRemove && c != '\n')
            result += c;
    fileName = result;

    textboxes[0].setTextLimit(100);
    textboxes[0].allowAlphaNumeric();

    textboxes[0].setTextFormat(sf::Color::Magenta, game->height / 20);
    textboxes[0].setBackgroundColor(sf::Color(12, 13, 23, 255));
    textboxes[0].setBorder(3, sf::Color(12, 13, 23, 255), sf::Color(173, 172, 173), sf::Color::Magenta);

    for (int i = 0; i < 2; i++)
    {
        myButtons.push_back(Button());
        myButtons[i].setFont(font);

        myButtons[i].setBorder(sf::Color(12, 13, 23, 255), 2);
        myButtons[i].setBackgroundColor(sf::Color(12, 13, 23, 255));
    }

    imagePreviewLabel.setString("Maze Preview: ");
    imagePreviewLabel.setFont(font);
    imagePreviewLabel.setFillColor(sf::Color::White);

    myButtons[0].setText("Export", sf::Color::Magenta);
    myButtons[1].setText("Back", sf::Color::Magenta);

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

    myButtons[0].setPosition(sf::Vector2f(game->width / 2.0f, game->height / 1.2f));

    myButtons[1].setPosition(sf::Vector2f(game->width * 0.05f, game->height * 0.05f));

    for (int i = 0; i < textboxes.size(); i++)
    {
        textboxes[i].setTextFormat(sf::Color::Magenta, game->height / 20);
        textboxes[i].setSize(sf::Vector2f(game->width * 0.3, textboxes[i].getCharacterSize() * 1.2 ));
    }

    textboxes[0].setPosition(sf::Vector2f(game->width * 0.025f,game->height * 0.2f));
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
}

void ExportImageScreen::draw()
{
    game->window->draw(background);
    game->window->draw(mazePreview);
    game->window->draw(imagePreviewLabel);

    for (int i = 0; i < myButtons.size(); i++)
        myButtons[i].render(game->window);

    for (int i = 0; i < textboxes.size(); i++)
        textboxes[i].draw(game->window);
}

void ExportImageScreen::previewMaze()
{
    Maze tempMaze = *maze;

    sf::Vector2f size(game->height / 2, game->height / 2);

    tempMaze.resize(size);
    tempMaze.setColors(sf::Color::White, sf::Color::Black, sf::Color::Black);
    tempMaze.setPosition(sf::Vector2f(0, 0));

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

void ExportImageScreen::exportMazeToPNG()
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