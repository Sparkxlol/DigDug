#include "DigDug.h"

enum Input { up, down, left, right, z, none };


DigDug::DigDug() : DigDug(nullptr, nullptr, sf::Vector2f(0, 0), nullptr)
{
	speed = 0.0f;
	input = none;
}


DigDug::~DigDug()
{

}


DigDug::DigDug(Spritesheet* s, Spritesheet* shotS, sf::Vector2f pos, sf::RenderWindow* win)
	: shot(shotS, pos, win), GameObject(s, pos, win) 
{
	speed = .5f;
	input = none;
}


// Runs the shoot method from shot.
void DigDug::shoot()
{

}


void DigDug::update()
{
	// Checks player inputs

	// Checks collision

	// Updates animator
}


// Draws digDug but also calls the shot's drawObject.
void DigDug::drawObject()
{

}


void DigDug::collide()
{
	// Checks collision with sand using input of direction.
	// If alligned properly with sand move toward sand, if not
	// move the direction of previous input, based on sand
	// mask. When moving towards sand, should run sand mask variable
	// with player direction and position. 

	// Checks collision with enemy, dies if touches.

	// Checks collision with rock, dies if rock is falling.

}


// Takes input for movement using the arrow keys
// and takes input to shoot using the Z key
void DigDug::playerInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		input = right;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		input = left;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		input = up;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		input = down;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		input = z;
	else
		input = none;

	// If player presses Z, shoot the shot and prevent movement.
}