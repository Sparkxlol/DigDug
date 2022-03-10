#include "Pooka.h"
#include "Game.h"


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
	collide();

	// Update movement
	if (getCurrentPump() <= 0)
	{
		movement();
		// Set animator to correct animation based on movement direction
		if (getDirection() == right)
			spritesheet.loadSprite(0);
		else
			spritesheet.loadSprite(7);
	}
	else
		pumpUpdate();

	// Update animator
	
}