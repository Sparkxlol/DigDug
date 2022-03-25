#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(15 * 16, 14 * 16), "DigDug");

    sf::Clock clock;
    int frames = 0;

    Game* game = new Game(&window);

    window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().height - 200,
        sf::VideoMode::getDesktopMode().height - 200));
    window.setFramerateLimit(144);


    //centers window on screen
    window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width * 0.5
        - window.getSize().x * 0.5, sf::VideoMode::getDesktopMode().height * 0.5
        - window.getSize().y * 0.5));

    srand(time(NULL));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            window.close();

        if (clock.getElapsedTime().asSeconds() > 1)
        {
            std::cout << frames << "\n";
            clock.restart();
            frames = 0;
        }

        window.clear();
        game->update();
        window.display();
        frames++;
    }

    delete game;

    return 0;
}