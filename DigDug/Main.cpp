#include <SFML/Graphics.hpp>
#include "Spritesheet.h"
#include "DigDug.h"
#include "Animator.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(14 * 16, 15 * 16), "DigDug");
    Spritesheet spriteSheet("Images/digDugSpritesheet.png", sf::Vector2i(256, 80), sf::Vector2i(16, 16));
    Spritesheet spriteSheet2("Images/shotSpritesheet.png", sf::Vector2i(64, 32), sf::Vector2i(16, 16));
    //spriteSheet.setPosition(100, 100);

    DigDug player(&spriteSheet, &spriteSheet2, sf::Vector2f(0,0), &window);

    window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().height - 200, sf::VideoMode::getDesktopMode().height - 200));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update();

        window.clear();
        player.drawObject();
        window.display();
    }

    return 0;
}