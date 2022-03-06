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


float Enemy::getSpeed()
{
	return speed;
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
	for (int i = 0; i < 4; i++)
		sandCollided[i] = false;

	sf::FloatRect topCollider = getCollider();
	topCollider.top -= .25f;

	sf::FloatRect bottomCollider = getCollider();
	bottomCollider.height += .25f;

	sf::FloatRect leftCollider = getCollider();
	leftCollider.left -= .25f;

	sf::FloatRect rightCollider = getCollider();
	rightCollider.width += .25f;

	// Performance tanker...
	for (int i = 0; i < game->getArrLength(Game::Object::sandSand); i++)
	{
		if (game->getActive(Game::Object::sandSand, i))
		{
			if (game->getSandPointer(i)->checkTopCollider(topCollider))
				sandCollided[0] = true;
			if (game->getSandPointer(i)->checkTopCollider(bottomCollider))
				sandCollided[1] = true;
			if (game->getSandPointer(i)->checkTopCollider(leftCollider))
				sandCollided[2] = true;
			if (game->getSandPointer(i)->checkTopCollider(rightCollider))
				sandCollided[3] = true;
		}
	}
}


void Enemy::movement()
{
	int moveDir = -1;
	float rockDifference;

	if (sandCollided[getDirection()])
	{
		for (int i = 0; i < game->getArrLength(Game::Object::rock); i++)
		{
			if (game->getRockPointer(i)->getFall())
			{
				rockDifference = getPosition().x - game->getRockPointer(i)->getPosition().x;
				if (rockDifference <= 16.0f && rockDifference >= -16.0f)
				{
					moveDir = moveFromRock(game->getRockPointer(i)->getPosition());
				}
			}
		}

		if (moveDir == -1)
		{
			int randomChoice = rand() % 15 + 1;

			if (escapeTimer.getElapsedTime().asSeconds() > 30.0f)
				moveDir = escapeLevel();
			else if (randomChoice > 1)
				moveDir = moveTowardPlayer();
			else if (randomChoice <= 1)
				moveDir = moveAwayPlayer();
		}
	}
	else
		moveDir = getDirection();

	switch (moveDir)
	{
	case up:
		move(sf::Vector2f(0, -getSpeed()));
		break;
	case down:
		move(sf::Vector2f(0, getSpeed()));
		break;
	case left:
		move(sf::Vector2f(-getSpeed(), 0));
		break;
	case right:
		move(sf::Vector2f(getSpeed(), 0));
		break;
	default:
		std::cout << "Invalid enemy movement direction!\n";
	}

	setDirection(moveDir);
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
		moveDir = moveRandom();

	return moveDir;
}


int Enemy::moveAwayPlayer()
{
	sf::Vector2f playerPos = game->getDigDugPointer()->getPosition();
	sf::Vector2f currentPos = getPosition();
	int moveDir = -1;

	if (playerPos.y > currentPos.y && !sandCollided[up])
		moveDir = up;
	else if (playerPos.y < currentPos.y && !sandCollided[down])
		moveDir = down;
	else if (playerPos.x > currentPos.x && !sandCollided[left])
		moveDir = left;
	else if (playerPos.x < currentPos.x && !sandCollided[right])
		moveDir = right;
	else
		moveDir = moveRandom();

	return moveDir;
}


int Enemy::moveRandom()
{
	// Might have slight up -> right bias due to this method.
	int randomDir[4] = { up, down, left, right };

	for (int i = 0; i < 4; i++)
	{
		int randNum = rand() % 4;
		int temp = randomDir[i];
		randomDir[i] = randomDir[randNum];
		randomDir[randNum] = temp;
	}

	for (int i = 0; i < 4; i++)
	{
		if (!sandCollided[randomDir[i]])
			return randomDir[i];
	}

	return -1;
}


int Enemy::moveFromRock(sf::Vector2f rockPos)
{
	int moveDir = -1;

	if (!sandCollided[left])
		moveDir = left;
	else if (!sandCollided[right])
		moveDir = right;
	else if (!sandCollided[down])
		moveDir = down;
	else if (!sandCollided[up])
		moveDir = up;

	return moveDir;
}


int Enemy::escapeLevel()
{
	int moveDir = -1;

	if (!sandCollided[up])
		moveDir = up;
	else if (!sandCollided[left])
		moveDir = left;
	else if (!sandCollided[right])
		moveDir = right;
	else if (!sandCollided[down])
		moveDir = down;

	return moveDir;
}
