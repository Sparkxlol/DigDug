#include "Shot.h"


Shot::Shot() : Shot(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/shotSpritesheet.png",
		sf::Vector2i(64, 32), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	setActive(false);
}


Shot::~Shot()
{
	// Deconstructor 
}


Shot::Shot(sf::RenderWindow* win, Game* game)
	: GameObject(win, game)
{
	spritesheet.setupSprite("Images/shotSpritesheet.png",
		sf::Vector2i(64, 32), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	setActive(false);
}


// "Shoots" the air pump, setting the shot active and checking collsion.
void Shot::shoot(sf::Vector2f playerPos, int direction)
{
	// Set position to playerPos
	setPosition(playerPos);

	// Sets active
	setActive(true);

	// Set animation to shoot in direction
	setDirection(direction);
	anim.setAnimation(2, 3, .2f, true);
}


void Shot::update()
{
	// Checks collision
	collide();

	// Updates the animator
	anim.playAnimation();

	// If animation is done and nothing is hit, set not active
	if (anim.getFinished())
		setActive(false);
}


void Shot::collide()
{
	// Checks collision with enemy, if collides, "attack" enemy.
}