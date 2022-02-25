#include "Fire.h"


Fire::Fire() : Fire(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/fireSpritesheet.png",
		sf::Vector2i(96, 32), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
}


Fire::~Fire()
{ 
	// Deconstructor
}


Fire::Fire(sf::RenderWindow* win, Game* game)
	: GameObject(win, game)
{
	spritesheet.setupSprite("Images/fireSpritesheet.png",
		sf::Vector2i(96, 32), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
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