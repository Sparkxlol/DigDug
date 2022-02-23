#include "Shot.h"


Shot::Shot() : Shot(nullptr, sf::Vector2f(0, 0), nullptr)
{
	setActive(false);
}


Shot::~Shot()
{
	// Deconstructor 
}


Shot::Shot(Spritesheet* s, sf::Vector2f playerPos, sf::RenderWindow* win)
	: GameObject(s, playerPos, win)
{
	setActive(false);
}


// "Shoots" the air pump, setting the shot active and checking collsion.
void Shot::shoot(sf::Vector2f playerPos, int direction)
{
	// Set position to playerPos

	// Sets active

	// Set animation to shoot in direction
}


void Shot::update()
{
	// Checks collision

	// Updates the animator

	// If animation is done and nothing is hit, set not active
}


void Shot::collide()
{
	// Checks collision with enemy, if collides, "attack" enemy.
}