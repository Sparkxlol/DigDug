#include "Fire.h"


Fire::Fire() : Fire(nullptr, sf::Vector2f(0, 0), nullptr)
{

}


Fire::~Fire()
{ 
	// Deconstructor
}


Fire::Fire(Spritesheet* s, sf::Vector2f fygarPos, sf::RenderWindow* win)
	: GameObject(s, fygarPos, win)
{

}


void Fire::shoot(sf::Vector2f fygarPos, int direction)
{
	// Set position to fygarPos

	// Set active

	// Set animator to correct animation
}


void Fire::update()
{
	// Check collision

	// If active run animation
}


void Fire::collide()
{
	// Check collision with player
}