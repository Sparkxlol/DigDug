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
// Input can be a negative number (Owen)
void Enemy::changeCurrentPump(int pump)
{
	currentPump += pump;
}


bool Enemy::getSandCollided()
{
	return sandCollided;
}


void Enemy::collide()
{
	sandCollided = false;

	for (int i = 0; i < game->getArrLength(Game::Object::sand); i++)
	{
		if (getCollider().intersects(game->getCollider(Game::Object::sand2, i)))
		{
			if (game->getSandPointer(i)->getTopActive())
				sandCollided = true;
		}
	}
}


float Enemy::getSpeed()
{
	return speed;
}
