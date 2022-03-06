#include "Fire.h"
#include "Game.h"


Fire::Fire() : Fire(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/fireSpritesheet.png",
		sf::Vector2i(96, 32), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	setActive(false);
	frame = 0;
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
	setActive(false);
	frame = 0;
}


void Fire::shoot(sf::Vector2f fygarPos, int direction)
{
	setPosition(fygarPos);

	// Set active
	setActive(true);
	setDirection(direction);
	spritesheet.setSize(sf::Vector2i(16, 16), sf::Vector2i(0, 0), 0);

	// Set animator to correct animation
	if (getDirection() == right)
	{
		setPosition(sf::Vector2f(getPosition().x + 16, getPosition().y));
		spritesheet.loadSprite(0);
	}
	else
	{
		setPosition(sf::Vector2f(getPosition().x - 16, getPosition().y));
		spritesheet.loadSprite(6);
	}

	frame = 0;
	clock.restart();
}


void Fire::update()
{
	// Check collision
	collide();

	if (clock.getElapsedTime().asSeconds() >= .33f)
	{
		frame++;

		if (frame == 3)
			setActive(false);

		int startX = (frame == 1) ? 16 : 48;

		if (getDirection() == right)
		{
			spritesheet.setSize(sf::Vector2i((frame + 1) * 16, 16), sf::Vector2i(startX, 0), 0);
		}
		else if (getDirection() == left);
		{
			move(sf::Vector2f(-16, 0));
			spritesheet.setSize(sf::Vector2i((frame + 1) * 16, 16), sf::Vector2i(startX, 16), 0);
		}
	}
}


void Fire::collide()
{
	if (game->checkCollision(getCollider(), Game::Object::dig, 0))
	{
		game->getDigDugPointer()->die();
	}
}