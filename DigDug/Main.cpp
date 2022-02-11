#include <SFML/Graphics.hpp>
#include "Spritesheet.h"
#include "DigDug.h"
#include "Animator.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(14 * 16, 15 * 16), "DigDug");
    Spritesheet spriteSheet("Images/digDugSpritesheet.png", sf::Vector2i(256, 80), sf::Vector2i(16, 16));
    //spriteSheet.setPosition(100, 100);

    DigDug player(&spriteSheet, );

    Animator anim(&spriteSheet);
    anim.setAnimation(0, 7, .5f, true);

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

        anim.playAnimation();

        window.clear();
        window.draw(spriteSheet);
        window.display();
    }

    return 0;
}