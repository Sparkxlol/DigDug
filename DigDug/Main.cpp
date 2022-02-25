#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(14 * 16, 15 * 16), "DigDug");

    sf::Clock clock;
    int frames = 0;

    Game* game = new Game(&window);

    window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().height - 200, sf::VideoMode::getDesktopMode().height - 200));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

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