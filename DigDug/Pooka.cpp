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
	: Enemy(win, game, EnemyType::pooka)
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
	anim.playAnimation();
}


void Pooka::reset(sf::Vector2f pos)
{
	Enemy::reset(pos);

	anim.setAnimation(0, 1, .2f, false);
}