#include "Enemy.h"


Enemy::Enemy() : Enemy(nullptr, nullptr)
{
	
}


Enemy::~Enemy()
{
	// Deconstructor
}


Enemy::Enemy(sf::RenderWindow* win, Game* game)
	: GameObject(win, game), currentPump(0), canFloat(false)
{
	
}


// Changes the pump amount
// Input can be a negative number (Owen)
void Enemy::changeCurrentPump(int pump)
{
	currentPump += pump;
}