#include "Shot.h"
#include "Game.h"


Shot::Shot() : Shot(nullptr, nullptr)
{
	spritesheet.setupSprite("Images/shotSpritesheet.png",
		sf::Vector2i(64, 48), sf::Vector2i(64, 48));
	anim.setSprite(&spritesheet);
	reset(sf::Vector2f(0, 0));
}


Shot::~Shot()
{
	// Deconstructor 
}


Shot::Shot(sf::RenderWindow* win, Game* game)
	: GameObject(win, game)
{
	spritesheet.setupSprite("Images/shotSpritesheet.png",
		sf::Vector2i(8, 48), sf::Vector2i(8, 0));
	anim.setSprite(&spritesheet);
	reset(sf::Vector2f(0, 0));
}


// "Shoots" the air pump, setting the shot active and checking collsion.
void Shot::shoot(sf::Vector2f playerPos, int direction)
{
	// Set position to playerPos
	setPosition(playerPos);
	this->playerPos = playerPos;

	// Sets active
	setActive(true);
	setDirection(direction);
	currentMask = 3;
}


void Shot::update()
{
	if (getActive())
	{
		collide();

		if (currentMask <= 48)
		{
			if (currentMask % 3 == 0)
			{
				switch (getDirection())
				{
				case up:
					spritesheet.setTextureRect(
						sf::IntRect(0, 0, 8, currentMask));
					setPosition(sf::Vector2f(playerPos.x + 6,
						playerPos.y + 4 - currentMask));
					break;
				case down:
					spritesheet.setTextureRect(
						sf::IntRect(8,48 - currentMask, 8, currentMask));
					setPosition(sf::Vector2f(playerPos.x + 6,
						playerPos.y + 12));
					break;
				case left:
					spritesheet.setTextureRect(
						sf::IntRect(16, 0,currentMask, 8));
					setPosition(sf::Vector2f(playerPos.x + 4 - currentMask,
						playerPos.y + 6));
					break;
				case right:
					spritesheet.setTextureRect(
						sf::IntRect(64 - currentMask, 8, currentMask, 8));
					setPosition(sf::Vector2f(playerPos.x + 12,
						playerPos.y + 6));
					break;
				default:
					std::cerr << "Invalid shot direction!\n";
				}
			}
			currentMask++;
		}
		else
			setActive(false);
	}
}


void Shot::collide()
{
	// Checks collision with enemy, if collides, "attack" enemy.
	for (int i = 0; i < game->getArrLength(Game::Object::fygar); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::fygar, i))
		{
			game->getFygarPointer(i)->changeCurrentPump(1);
			currentMask = 0;
			setActive(false);
			return;
		}
	}

	for (int i = 0; i < game->getArrLength(Game::Object::pooka); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::pooka, i))
		{
			game->getPookaPointer(i)->changeCurrentPump(1);
			currentMask = 0;
			setActive(false);
			return;
		}
	}

	// Checks collision with sand, if collides, shot stops
	for (int i = 0;i < game->getArrLength(Game::Object::sandSand);i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::sandSand, i))
		{
			currentMask = 0;
			setActive(false);
			return;
		}
	}
}


void Shot::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	setActive(false);
	currentMask = 3;
	playerPos = pos;
}