#include "Enemy.h"
#include "Game.h"


Enemy::Enemy() : Enemy(nullptr, nullptr)
{
	sandCollided = false;
	speed = 0.0f;
}


Enemy::~Enemy()
{
	// Deconstructor
}


Enemy::Enemy(sf::RenderWindow* win, Game* game)
	: GameObject(win, game), currentPump(0), canFloat(false)
{
	sandCollided = false;
	speed = .25f;
}


// Changes the pump amount
// Input can be a negative number 
void Enemy::changeCurrentPump(int pump)
{
	currentPump += pump;
	pumpClock.restart();

	// Must implement direction based exploding as well as move object to left/right when big
	if (currentPump > 1)
	{
		if (currentPump == 2)
			spritesheet.setSize(sf::Vector2i(32, 32), sf::Vector2i(16, 32), currentPump - 2);
		else
			spritesheet.loadSprite(currentPump - 2);
	}
	else
	{
		spritesheet.setSize(sf::Vector2i(16, 16), sf::Vector2i(0, 0), 0);
		if (pump == 1)
		{
			spritesheet.loadSprite(14);
		}
	}
}


void Enemy::die(std::string type)
{
	setActive(false);
	game->createScore(getPosition(), type);
}


int Enemy::getCurrentPump()
{
	return currentPump;
}


bool Enemy::getSandCollided()
{
	return sandCollided;
}


void Enemy::pumpUpdate()
{
	if (getCurrentPump() > 0)
	{
		if (getCurrentPump() > 3)
			die("pump");
		if (pumpClock.getElapsedTime().asSeconds() > 1.0f)
		{
			changeCurrentPump(-1);
			pumpClock.restart();
		}
	}
}


void Enemy::collide()
{
	sandCollided = false;

	for (int i = 0; i < game->getArrLength(Game::Object::sandSand); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::sandSand, i))
		{
			sandCollided = true;
		}
	}
}


float Enemy::getSpeed()
{
	return speed;
}
