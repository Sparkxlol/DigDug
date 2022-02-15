#include "Pooka.h"


Pooka::Pooka() : Pooka(nullptr, sf::Vector2f(0, 0), nullptr)
{

}


Pooka::~Pooka()
{
	// Deconstructor
}


Pooka::Pooka(Spritesheet* s, sf::Vector2f pos, sf::RenderWindow* win)
	: Enemy(s, pos, win)
{

}


void Pooka::update()
{
	// Update movement

	// Check collisions

	// Update animator
}


void Pooka::collide()
{
	//
}


void Pooka::movement()
{
	// Moves until hits wall, using collision checks

	// When stuck floats periodically

	// If in range of player moves toward player and attacks

	// -- Might need to watch more gameplay to flesh-out -- 
}