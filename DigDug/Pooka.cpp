#include "Pooka.h"


Pooka::Pooka() : Pooka(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/pookaSpritesheet.png",
		sf::Vector2i(112, 96), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
}


Pooka::~Pooka()
{
	// Deconstructor
}


Pooka::Pooka(sf::RenderWindow* win, Game* game)
	: Enemy(win, game)
{
	spritesheet.setupSprite("Images/pookaSpritesheet.png",
		sf::Vector2i(112, 96), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
}


void Pooka::update()
{
	// Check collisions

	// Update movement

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