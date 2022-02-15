#include "Fygar.h"


Fygar::Fygar() : Fygar(nullptr, nullptr, sf::Vector2f(0, 0), nullptr)
{

}


Fygar::~Fygar()
{
	// Deconstructor
}


Fygar::Fygar(Spritesheet* s, Spritesheet* fireS, sf::Vector2f pos, sf::RenderWindow* win)
	: Enemy(s, pos, win), fire(fireS, pos, win)
{

}


void Fygar::shootFire()
{
	// Sets fire active and uses its shoot method
}


void Fygar::update()
{
	// Update movement

	// Check collisions

	// Update animator

	// Updates fire
}


void Fygar::drawObject()
{
	// Calls overrided draw, but also calls the fires
}


void Fygar::collide()
{
	//
}


void Fygar::movement()
{
	// Moves until hits wall, using collision checks

	// When stuck floats periodically

	// If in range of player moves toward player and shoots fire.
}