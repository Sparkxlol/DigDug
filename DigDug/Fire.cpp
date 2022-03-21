#include "Fire.h"
#include "Game.h"


Fire::Fire() : Fire(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/fireSpritesheet.png",
		sf::Vector2i(96, 32), sf::Vector2i(16, 16));
	anim.setSprite(&spritesheet);
	reset(sf::Vector2f(0, 0));
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
	reset(sf::Vector2f(0, 0));
}


// Shoots fire in the corresponding direction and position.
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


// Updates the fire to correct size and deletion.
void Fire::update()
{
	// Check collision
	collide();

	// Every .33 seconds the frame changes.
	if (clock.getElapsedTime().asSeconds() >= .33f)
	{
		frame++;

		// Max frame.
		if (frame == 3)
			setActive(false);

		int startX = (frame == 1) ? 16 : 48;

		// Increaes size of frame and moves it to correct location.
		if (getDirection() == right)
		{
			spritesheet.setSize(sf::Vector2i((frame + 1) * 16, 16), sf::Vector2i(startX, 0), 0);
		}
		else if (getDirection() == left)
		{
			move(sf::Vector2f(-16, 0));
			spritesheet.setSize(sf::Vector2i((frame + 1) * 16, 16), sf::Vector2i(startX, 16), 0);
		}

		clock.restart();
	}
}


// Checks collisions with digDug, killing the player if hit.
void Fire::collide()
{
	if (game->checkCollision(getCollider(), Game::Object::dig, 0))
	{
		game->getDigDugPointer()->die("enemy");
	}
	for (int i = 0;i<game->getArrLength(Game::Object::sandSand);i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::sandSand, i))
		{
			setActive(false);
		}
	}
}


// Resets the fire to default values.
void Fire::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	setActive(false);
	clock.restart();
	frame = 0;
}