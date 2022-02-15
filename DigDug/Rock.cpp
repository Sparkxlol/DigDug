#include "Rock.h"

Rock::Rock() : Rock(nullptr, sf::Vector2f(0, 0), nullptr)
{ 
}


Rock::~Rock()
{

}


Rock::Rock(Spritesheet* s, sf::Vector2f pos, sf::RenderWindow* win)
	: GameObject(s, pos, win)
{ 
	isFalling = false;
}



void Rock::startFall()
{
	// Play initial animation 
}


void Rock::fall()
{
	// Set is falling to true, and run falling animation.
}


void Rock::update()
{
	// Check collisions

	// If rock started falling, check when animation is done
	// and run fall.
}


void Rock::collide()
{
	// Check collision of sand under rock, if doesn't collide, fall.

	// Check collision with enemies/player, run death functions.
}
