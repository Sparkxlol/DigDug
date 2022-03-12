#include "Enemy.h"
#include "Game.h"


Enemy::Enemy() : Enemy(nullptr, nullptr, EnemyType::pooka)
{
	reset(sf::Vector2f(0, 0));
}


Enemy::~Enemy()
{
	// Deconstructor
}


Enemy::Enemy(sf::RenderWindow* win, Game* game, EnemyType type)
	: GameObject(win, game), type(type)
{
	reset(sf::Vector2f(0, 0));
}


// Changes the pump amount and changes sprite.
// Input can be a negative number 
void Enemy::changeCurrentPump(int pump)
{
	// If not maxes out pump then change sprite.
	if (currentPump < 4)
	{
		currentPump += pump;
		pumpClock.restart();

		// Must implement direction based exploding as well as move object to left/right when big
		// If greater than 0 pumps, make sprite larger.
		if (currentPump > 0)
		{
			spritesheet.setSize(sf::Vector2i(32, 32), sf::Vector2i(0, 32), currentPump - 1);
			anim.setActive(false);
		}
		// If 0 pumps, make sprite normal size.
		else
		{
			spritesheet.setSize(sf::Vector2i(16, 16), sf::Vector2i(0, 0), 0);
			anim.setActive(true);
		}
	}
}


// Kills enemy and creates a score with specific death type,
// setting enemy not active.
void Enemy::die(std::string type)
{
	setActive(false);
	game->createScore(getPosition(), type);
}


// Gets the amount that an enemy is "pumped up".
int Enemy::getCurrentPump()
{
	return currentPump;
}


// Gets if sand is collided at all.
bool Enemy::getSandCollided()
{
	return sandCollided;
}


// Gets current speed of enemy.
float Enemy::getSpeed()
{
	return speed;
}


// Gets if the enemy can float.
bool Enemy::getFloat()
{
	return canFloat;
}


// Updates pump based on current pump value.
void Enemy::pumpUpdate()
{
	if (getCurrentPump() > 0)
	{
		if (getCurrentPump() > 3)
		{
			// If current pump is 3, max pump out to 4.
			if (getCurrentPump() == 3)
			{
				pumpClock.restart();
				currentPump = 4;
			}
			// If last sprite has been active for .5f seconds then die.
			else if (getCurrentPump() == 4 && pumpClock.getElapsedTime().asSeconds() > .5f)
				die("pump");
		}
		// If pump clock is greater than 1, deflate the enemy by 1.
		if (pumpClock.getElapsedTime().asSeconds() > 1.0f && currentPump < 4)
		{
			changeCurrentPump(-1);
			pumpClock.restart();
		}
	}
}


// Checks collisions with sand on every side.
// !!! Could be very much optimized... !!! Perhaps multi-dimensional array >.<
void Enemy::collide()
{
	for (int i = 0; i < 4; i++)
		sandCollided[i] = false;

	// Creates 4 colliders for each side of the enemy.
	sf::FloatRect topCollider = getCollider();
	topCollider.top -= .25f;

	sf::FloatRect bottomCollider = getCollider();
	bottomCollider.height += .25f;

	sf::FloatRect leftCollider = getCollider();
	leftCollider.left -= .25f;

	sf::FloatRect rightCollider = getCollider();
	rightCollider.width += .25f;

	// Checks each collision with every active sand.
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


/* Allows player to move with many different choices:
* moves towards player, moves away from player, float towards player,
* moves randomly, escapes the level, and runs from falling rocks. */
void Enemy::movement()
{
	// 1/randomFloatTime to float on every frame.
	static const int randomFloatPercent = 5000;
	static const float randomFloatTime = 10.0f;
	int moveDir = -1; // Direction to move.
	float rockDifference; // Distance from the rock.
	int randFloat = rand() % randomFloatPercent + 1;
	bool formerCanFloat = canFloat; // Checks if enemy was just floating.

	// If floating then moveFloat.
	if ((randFloat == randomFloatPercent
		&& escapeTimer.getElapsedTime().asSeconds() > randomFloatTime)
		|| canFloat)
		moveDir = moveFloat();
	// If cannot keep moving in current direction, change move type.
	else if (sandCollided[getDirection()]) // !!! Might want to change to every 16x16 block for more varience. !!!
	{
		// Checks for each rock if enemy is near, and if so, run from the rock.
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

		// If time after level load is 30 seconds, escape from level.
		// If not, randomChance/15 chance to move towards player,
		// otherwise move away.
		if (moveDir == -1)
		{
			static const int randomChoice = rand() % 15 + 1;

			if (escapeTimer.getElapsedTime().asSeconds() > 30.0f)
				moveDir = escapeLevel();
			else if (randomChoice > 1)
				moveDir = moveTowardPlayer();
			else if (randomChoice <= 1)
				moveDir = moveAwayPlayer();
		}
	}
	else // If not stopped, keep moving in current direction.
		moveDir = getDirection();
	
	// If not floating and either was just floating or direction has changed,
	// change the animation to the corresponding tiles.
	if (!canFloat && (getDirection() != moveDir || formerCanFloat != canFloat))
	{
		if (moveDir == left)
		{
			if (type == EnemyType::pooka)
				anim.setAnimation(7, 8, .2f, true);
			else
				anim.setAnimation(8, 9, .2f, true);
		}
		else
			anim.setAnimation(0, 1, .2f, true);
	}

	// Move in the direction decided by previous functions.
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


// Moves toward the player using position of digDug and current collisions.
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
	else // If cannot move towards player, move randomly.
		moveDir = moveRandom();

	return moveDir;
}


// Move away from player using position and collisions.
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


// Chooses a random direction to move in.
int Enemy::moveRandom()
{
	// Might have slight up -> right bias due to this method.
	int randomDir[4] = { up, down, left, right };

	// Randomizes the order of the direction array.
	for (int i = 0; i < 4; i++)
	{
		int randNum = rand() % 4;
		int temp = randomDir[i];
		randomDir[i] = randomDir[randNum];
		randomDir[randNum] = temp;
	}

	// Uses randomized array to move if can based on collision.
	for (int i = 0; i < 4; i++)
	{
		if (!sandCollided[randomDir[i]])
			return randomDir[i];
	}

	return -1;
}


// Moves away from the rock based on the passed position.
int Enemy::moveFromRock(sf::Vector2f rockPos)
{
	int moveDir = -1;

	// Tries to move left or right then down to avoid rock.
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


// Moves toward the top of the screen.
// Generally moves up then left when enemy reaches the top.
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


// If enemy is floating moves towards the player.
int Enemy::moveFloat()
{
	// !!! Doesn't work properly !!!
	// If ever close to player during current flight, move to former position.
	// Otherwise move towards the general position of the player.
	sf::Vector2f playerPos = (floatTarget.x != -1)
		? floatTarget : game->getDigDugPointer()->getPosition();
	sf::Vector2f currentPos = getPosition();
	bool nearPlayer = false;

	// Checks if the enemy is near the player and sets the target to the former playerPos.
	if (abs(currentPos.x - playerPos.x) < 32 && abs(currentPos.y - playerPos.y) < 32)
	{
		nearPlayer = true;
		if (floatTarget.x != -1)
			floatTarget = playerPos;
	}

	// If enemy has just started floating,
	// set the corresponding animation and reset target.
	if (canFloat == false)
	{
		floatTarget = sf::Vector2f(-1, -1);
		canFloat = true;
		if (type == EnemyType::pooka)
			anim.setAnimation(3, 4, .5f, true);
		else if (type == EnemyType::fygar)
			anim.setAnimation(6, 7, .5f, true);
	}

	// If not colliding at all and at any time near the player, stop floating.
	if (!sandCollided[0] && !sandCollided[1] &&
		!sandCollided[2] && !sandCollided[3] && nearPlayer)
		canFloat = false;

	// If still floating move towards player ignoring collisions,
	// otherwise move towards player regarding collision.
	if (canFloat)
	{
		int moveDir = -1;

		if (playerPos.y < currentPos.y)
			moveDir = up;
		else if (playerPos.y > currentPos.y)
			moveDir = down;
		else if (playerPos.x < currentPos.x)
			moveDir = left;
		else if (playerPos.x > currentPos.x)
			moveDir = right;
		else
			moveDir = -1;

		return moveDir;
	}
	else
	{
		floatTarget = sf::Vector2f(-1, -1);
		return moveTowardPlayer();
	}
}


// Resets player to default values.
void Enemy::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	pumpClock.restart();
	escapeTimer.restart();
	canFloat = false;
	floatTarget = sf::Vector2f(-1, -1);
	currentPump = 0;
	speed = .25f;

	for (int i = 0; i < 4; i++)
		sandCollided[i] = false;
}