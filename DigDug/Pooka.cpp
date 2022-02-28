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
	collide();

	// Update movement
	movement();

	// Update animator
	
}


void Pooka::collide()
{
	Enemy::collide();
}


void Pooka::movement()
{
	std::cout << getSandCollided() << std::endl;

	// Moves until hits wall, using collision checks
	if (getSandCollided())
	{
		switch (getDirection())
		{
		case up:
			setDirection(down);
			break;
		case down:
			setDirection(up);
			break;
		case left:
			setDirection(right);
			break;
		case right:
			setDirection(left);
			break;
		}
	}

	sf::Vector2f upV = sf::Vector2f(0, -getSpeed());
	sf::Vector2f downV = sf::Vector2f(0, getSpeed());
	sf::Vector2f leftV = sf::Vector2f(-getSpeed(), 0);
	sf::Vector2f rightV = sf::Vector2f(getSpeed(), 0);

	switch (getDirection())
	{
	case up:
		move(upV);
		break;
	case down:
		move(downV);
		break;
	case left:
		move(leftV);
		break;
	case right:
		move(rightV);
		break;
	}

	// When stuck floats periodically

	// If in range of player moves toward player and attacks

	// -- Might need to watch more gameplay to flesh-out -- 
}