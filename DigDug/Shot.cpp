#include "Shot.h"

Shot::Shot(Spritesheet* s, sf::Vector2f playerPos, sf::RenderWindow* win)
	: GameObject(s, playerPos, win)
{

}


Shot::~Shot()
{
	// Deconstructor 
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
	// Updates the animator

	// If animation is done and nothing is hit, set not active

	// Checks collision
}


void Shot::collide()
{
	// Checks collision with enemy, if collides, "attack" enemy.
}