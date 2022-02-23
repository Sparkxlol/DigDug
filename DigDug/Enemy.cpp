#include "Enemy.h"


Enemy::Enemy() : Enemy(nullptr, sf::Vector2f(0, 0), nullptr)
{

}


Enemy::~Enemy()
{
	// Deconstructor
}


Enemy::Enemy(Spritesheet* s, sf::Vector2f pos, sf::RenderWindow* win)
	: GameObject(s, pos, win), currentPump(0), canFloat(false)
{

}


// Changes the pump amount
// Input can be a negative number (Owen)
void Enemy::changeCurrentPump(int pump)
{
	currentPump += pump;
}