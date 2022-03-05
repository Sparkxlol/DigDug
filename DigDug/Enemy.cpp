#include "Enemy.h"
#include "Game.h"


Enemy::Enemy() : Enemy(nullptr, nullptr)
{
	speed = 0.0f;
}


Enemy::~Enemy()
{
	// Deconstructor
}


Enemy::Enemy(sf::RenderWindow* win, Game* game)
	: GameObject(win, game), currentPump(0), canFloat(false)
{
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
	largeCollider = spritesheet.getGlobalBounds();
	boundingBox.left -= .125f;
	boundingBox.width += .25f;
	boundingBox.top -= .125f;
	boundingBox.height += .25f;

	for (int i = 0; i < game->getArrLength(Game::Object::sandSand); i++)
	{
		if (game->checkCollision(largeCollider, Game::Object::sandSand, i))
		{
			sf::Vector2f sandPos = game->getSandPointer(i)->getPosition();
			if (sandPos.y > getPosition().y)
				sandCollided[0] = true;
			else if (sandPos.y < getPosition().y)
				sandCollided[1] = true;
			else if (sandPos.x < getPosition().x)
				sandCollided[2] = true;
			else if (sandPos.x > getPosition().x)
				sandCollided[3] = true;
		}
	}
}


float Enemy::getSpeed()
{
	return speed;
}


int Enemy::moveTowardPlayer()
{
	sf::Vector2f playerPos = game->getDigDugPointer()->getPosition();
	sf::Vector2f currentPos = getPosition();
	int moveDir = -1;

	if (playerPos.y < currentPos.y && !sandCollided[up])
		moveDir = up;
	else if (playerPos.y > currentPos.y && !sandCollided[down])
		moveDir = down;
	else if (playerPos.x < currentPos.x && !sandCollided[left])
		moveDir = left;
	else if (playerPos.x > currentPos.x && !sandCollided[right])
		moveDir = right;
	else
	{
		int randNums[4] = { -1, -1, -1, -1 };
		int randNum;
		bool continueLoop;

		for (int i = 0; i < 4; i++)
		{
			do
			{
				continueLoop = false;
				randNum = rand() % 4;

				for (int j = 0; j < 4; j++)
				{
					if (randNums[j] == randNum)
						continueLoop = true;
				}
			} while (continueLoop);
		}
	}
}


int Enemy::moveAwayPlayer()
{

}


int Enemy::runFromRock()
{

}


int Enemy::escapeLevel()
{

}
