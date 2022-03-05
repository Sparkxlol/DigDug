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
		movement();
	else
		pumpUpdate();

	// Update animator
	
}


void Pooka::collide()
{
	Enemy::collide();
}


void Pooka::movement()
{
	int moveDir = up;

	//Get player position

	switch (moveDir)
	{
	case up:
		move(sf::Vector2f(0, -getSpeed()));
		break;
	case down:
		move(sf::Vector2f(0, getSpeed()));
		break;
	case left:
		move(sf::Vector2f(-getSpeed(), 0));
		break;
	case right:
		move(sf::Vector2f(getSpeed(), 0));
		break;
	default:
		std::cout << "Invalid enemy movement direction!\n";
	}

	// When stuck floats periodically

	// If in range of player moves toward player and attacks

	// -- Might need to watch more gameplay to flesh-out -- 
}