#include "DigDug.h"

DigDug::DigDug()
{
	//if you're here you are wrong
}


DigDug::~DigDug()
{

}


DigDug::DigDug(Spritesheet* s) : GameObject(s, , sf::RenderWindow* win))
{
	canMove = true;
	speed = .5f;
}

/*
Shot& DigDug::getShot()
{
	return shot;
}


void DigDug::shoot()
{
	
}
*/

void DigDug::update()
{
	
}


void DigDug::playerInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		move(sf::Vector2f(speed, 0));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		move(sf::Vector2f(speed, 0));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		move(sf::Vector2f(speed, 0));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		move(sf::Vector2f(speed, 0));
}