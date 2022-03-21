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
	// If already attached and have waited for .1 seconds
	// allow another pump, and if pump is over 3, kill enemy and set unactive.
	if (attached && shootWait.getElapsedTime().asSeconds() > .5f)
	{
		shootWait.restart();
		attachedEnemy->changeCurrentPump(1);
		if (attachedEnemy->getCurrentPump() > 3)
		{
			setActive(false);
			attached = false;
			attachedEnemy = nullptr;
		}
	}
	// If not attached and not yet active, reset position and direction
	// based on the parameters and update the mask to the correct amount.
	else if (!attached && !getActive())
	{
		// Set position to playerPos
		setPosition(playerPos);
		this->playerPos = playerPos;

		// Sets active
		setActive(true);
		setDirection(direction);
		currentMask = 3;
		updateMask();
	}

}


// Updates the shot, either checking for collisions if not attached
// or unattaching if the enemies pump is 0.
void Shot::update()
{
	if (getActive())
	{
		if (!attached)
		{
			collide();
			updateMask();
		}
		else
		{
			if (attachedEnemy->getCurrentPump() <= 0 
				|| attachedEnemy->getActive() == false)
			{
				setActive(false);
				attached = false;
				attachedEnemy = nullptr;
			}
		}
	}
}


// Checks collision with enemies to kill them
// or with sand to set non-active.
void Shot::collide()
{
	// Checks collision with enemy, if collides, "attack" enemy.
	for (int i = 0; i < game->getArrLength(Game::Object::enemy); i++)
	{
		if (game->checkCollision(getCollider(), Game::Object::enemy, i)
			&& game->getEnemyPointer(i)->getCurrentPump() <= 3)
		{
			// Sets the pump to attached and changes
			// the current pump of the collided enemy.
			attached = true;
			attachedEnemy = game->getEnemyPointer(i);
			attachedEnemy->changeCurrentPump(1);
			shootWait.restart();
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


// Updates the appearance of the shot, based on the direction it is facing.
void Shot::updateMask()
{
	if (currentMask <= 48)
	{
		// Only updates every 3 frames for more pixelated look.
		if (currentMask % 3 == 0)
		{
			/* Moves the initial starting position
			* of the shot to the correct place.
			* If facing down or right, the shot needs to appear
			* from the back of the sprite rather than the forwards. */
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
					sf::IntRect(8, 48 - currentMask, 8, currentMask));
				setPosition(sf::Vector2f(playerPos.x + 6,
					playerPos.y + 12));
				break;
			case left:
				spritesheet.setTextureRect(
					sf::IntRect(16, 0, currentMask, 8));
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


// Returns if the shot is attached.
bool Shot::getAttached()
{
	return attached;
}


// Resets the shot the defaults, un-attached.
void Shot::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	attached = false;
	attachedEnemy = nullptr;
	setActive(false);
	currentMask = 3;
	playerPos = pos;
}