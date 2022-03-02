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
	movement();

	// Update animator
	
}


void Pooka::collide()
{
	Enemy::collide();
}


void Pooka::movement()
{
	//std::cout << getSandCollided() << std::endl;

	//Get player position
	sf::Vector2f playerPos = game->getDigDugPointer()->getPosition();
	sf::Vector2f currentPos = getPosition();

	if (currentPos.x > playerPos.x)
	{
		if (currentPos.y > playerPos.y)
		{

		}
		else if(currentPos.y < playerPos.y)
		{

		}

		else
		{

		}
	}
	else if (currentPos.x < playerPos.x)
	{
		if (currentPos.y > playerPos.y)
		{

		}
		else if(currentPos.y < playerPos.y)
		{

		}
		else
		{

		}
	}
	else 
	{
		if (currentPos.y > playerPos.y)
		{

		}
		else if(currentPos.y < playerPos.y)
		{

		}
		else
		{

		}
	}

	sf::Vector2f upV = sf::Vector2f(0, -getSpeed());
	sf::Vector2f downV = sf::Vector2f(0, getSpeed());
	sf::Vector2f leftV = sf::Vector2f(-getSpeed(), 0);
	sf::Vector2f rightV = sf::Vector2f(getSpeed(), 0);

	// Moves until hits wall, using collision checks
	if (getSandCollided())
	{
		switch (getDirection())
		{
		case up:
			move(downV);
			setDirection(right);
			break;
		case down:
			move(upV);
			setDirection(left);
			break;
		case left:
			move(rightV);
			setDirection(up);
			break;
		case right:
			move(leftV);
			setDirection(down);
			break;
		}
	}
	else
	{
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
	}

	// When stuck floats periodically

	// If in range of player moves toward player and attacks

	// -- Might need to watch more gameplay to flesh-out -- 
}