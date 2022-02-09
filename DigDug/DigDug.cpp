#include <SFML/Graphics.hpp>
#include "Spritesheet.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(14 * 16, 15 * 16), "SFML works!");
    Spritesheet spriteSheet("Images/digDugSpritesheet.png", sf::Vector2i(256, 80), sf::Vector2i(16, 16));
    spriteSheet.setPosition(100, 100);
    spriteSheet.loadSprite(3);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(spriteSheet);
        window.display();
    }

    return 0;
}