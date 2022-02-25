#include "Rock.h"

Rock::Rock() : Rock(nullptr, nullptr)
{ 
	spritesheet.setupSprite("Images/rockSpritesheet.png",
		sf::Vector2i(64, 16), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	isFalling = false;
}


Rock::~Rock()
{

}


Rock::Rock(sf::RenderWindow* win, Game* game)
	: GameObject(win, game)
{ 
	spritesheet.setupSprite("Images/rockSpritesheet.png",
		sf::Vector2i(64, 16), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
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
