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
	// Prevents dead enemies from being pumped.
	if (deathType == "none")
	{
		if (currentPump == 0)
			initialPosition = getPosition();

		// If not maxes out pump then change sprite.
		if (currentPump < 4)
		{
			currentPump += pump;
			pumpClock.restart();

			// If greater than 0 pumps, make sprite larger.
			if (currentPump > 0)
			{
				setPosition(sf::Vector2f(initialPosition.x - 8, initialPosition.y));
				spritesheet.setSize(sf::Vector2i(32, 32),
					sf::Vector2i(0, 32), currentPump - 1);
				anim.setActive(false);
			}
			// If 0 pumps, make sprite normal size.
			else
			{
				setPosition(initialPosition);
				spritesheet.setSize(sf::Vector2i(16, 16), sf::Vector2i(0, 0), 0);
				anim.setActive(true);
			}
		}
	}
}


// Kills enemy and creates a score with specific death type,
// setting enemy not active.
void Enemy::die(std::string type)
{
	if (deathType == "none")
	{
		deathWait.restart();
		deathType = type;
		if (type == "pump")
		{
			game->createScore(getPosition(), type);
			setActive(false);
		}
		else if (type == "offscreen")
			setActive(false);
		else if (type == "rock") // Changes sprite to correct animation and size.
		{
			if (currentPump != 0)
				setPosition(initialPosition);
			spritesheet.setSize(sf::Vector2i(16, 16), sf::Vector2i(0, 0), 0);
			switch (getDirection())
			{
			case left:
				anim.setAnimation(10, 11, .5f, false);
			default:
				anim.setAnimation(2, 3, .5f, false);
			}
		}
	}
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


// Gets the way enemy died.
std::string Enemy::getDeathType()
{
	return deathType;
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
			else if (getCurrentPump() == 4 
				&& pumpClock.getElapsedTime().asSeconds() > .5f)
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


// Checks collision with surrounding sand.
void Enemy::collide()
{
	checkSurroundingSand(getPosition(), sandCollided);
}


/* Allows player to move with many different choices:
* moves towards player, moves away from player, float towards player,
* moves randomly, escapes the level, and runs from falling rocks. */
void Enemy::movement()
{
	if (getDeathType() == "none")
	{
		// 1/randomFloatTime to float on every frame.
		// Increases float change and decreases time every 12 rounds.
		int randomFloatPercent = 15 - static_cast<int>(game->getLevel() / 12);
		float randomFloatTime = 5.0f - (static_cast<int>(game->getLevel() / 12) * .25f);
		static const float escapeTime = 45.0f;
		int moveDir = -1; // Direction to move.
		float rockDifference; // Distance from the rock.
		bool formerCanFloat = canFloat; // Checks if enemy was just floating.
		sf::Vector2f rockPos; // Position of falling rock.
		bool runRock = false; // True if rock is near enemy and falling.

		// Checks for each rock if enemy is near, and if so, run from the rock.
		for (int i = 0; i < game->getArrLength(Game::Object::rock); i++)
		{
			if (game->getRockPointer(i)->getFall())
			{
				rockDifference = getPosition().x
					- game->getRockPointer(i)->getPosition().x;
				if (rockDifference <= 16.0f && rockDifference >= -16.0f)
				{
					runRock = true;
					currentCollides = 1;
					rockPos = game->getRockPointer(i)->getPosition();
				}
			}
		}

		// Top left of current square.
		sf::Vector2f pathOrigin = sf::Vector2f(
			static_cast<int>(getPosition().x)
			- (static_cast<int>(getPosition().x) % 16),
			static_cast<int>(getPosition().y)
			- (static_cast<int>(getPosition().y) % 16));

		// Finds the distance away from a 16 (from the top & left) in both axes.
		float xPos = static_cast<int>(getPosition().x) % 16 
			+ (getPosition().x - static_cast<int>(getPosition().x));
		float yPos = static_cast<int>(getPosition().y) % 16
			+ (getPosition().y - static_cast<int>(getPosition().y));

		// If xPos/yPos is close to 0 set to 0,
		// to check when digDug is close 
		// enough to the axis to move on that axis.
		xPos = (xPos < getSpeed() + getSpeed() / 2) ? 0.0f : xPos;
		yPos = (yPos < getSpeed() + getSpeed() / 2) ? 0.0f : yPos;

		// If floating then moveFloat.
		if (canFloat)
		{
			path.clear();
			moveDir = moveFloat();
		}
		// Near center of x and y and not in the last square as previous run.
		else if (xPos == 0 && yPos == 0 && pathOrigin != lastSquare)
		{
			lastSquare = pathOrigin;

			// If not doing random choices 
			// and at the end of the path, find new path.
			if (currentPath + 1 >= path.size() && currentCollides <= 0)
			{
				// Top left corner to standardize initial path position.
				path.clear();
				currentPath = 0;
				pathEndFound = false;
				// Finds path to escape the level at top left.
				if (escapeTimer.getElapsedTime().asSeconds() >= escapeTime)
					findTarget(sf::Vector2f(-16, 16), // Death spot to escape
						pathOrigin, 5, std::vector<int>());
				if (path.size() == 0) // Finds path to player.
					findTarget(game->getDigDugPointer()->getPosition(),
						pathOrigin, 5, std::vector<int>());
			}

			// If near falling rock move away from rock.
			if (runRock)
			{
				path.clear();
				moveDir = moveFromRock(rockPos);
				currentCollides--;
			}
			// If not randomly moving,
			// move towards the next position in path vector
			else if (pathEndFound && path.size() != 0 && currentCollides <= 0)
			{
				// Makes sure not out of range.
				if (currentPath < path.size())
				{
					// Make sure enemy doesn't go through walls.
					if (sandCollided[path.at(currentPath)])
						moveDir = moveTowardPlayer();				
					else
						moveDir = path.at(currentPath);
					currentPath++;
					if (currentPath + 1 == path.size())
						currentCollides = 1;
				}
				else
					moveDir = moveTowardPlayer();
			}
			// If collided with a wall and randomly moving,
			// choices random choice.
			else if (sandCollided[getDirection()])
			{
				path.clear();
				if (currentCollides > 0)
					currentCollides--;
				int randFloat = rand() % randomFloatPercent + 1;

				if (randFloat == randomFloatPercent &&
					escapeTimer.getElapsedTime().asSeconds() > randomFloatTime)
					moveDir = moveFloat();
				else
				{
					// If time after level load is 30 seconds, escape from level.
					// If not, randomChance/15 chance to move towards player,
					// otherwise move away.
					if (moveDir == -1)
					{
						static const int randomChoice = rand() % 15 + 1;

						if (escapeTimer.getElapsedTime().asSeconds() > escapeTime)
							moveDir = escapeLevel();
						else if (randomChoice <= 2)
							moveDir = moveAwayPlayer();
						else
							moveDir = moveTowardPlayer();
					}
				}
			}
			else
				moveDir = getDirection();
		}
		else // If not stopped, keep moving in current direction.
			moveDir = getDirection();

		// If not floating and either was just floating or direction has changed,
		// change the animation to the corresponding tiles.
		if (!canFloat &&
			(getDirection() != moveDir || formerCanFloat != canFloat))
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

		// If enemy is offscreen, it is killed.
		if (getPosition().x <= -15.5 || getPosition().x >= 15 * 16
			|| getPosition().y < 16 || getPosition().y >= 14 * 16)
			die("offscreen");

		// Floats if ever stuck/colliding.
		if (moveDir == -1)
			moveDir = moveFloat();

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
	else
	{
		if (deathType == "rock")
		{
			// If not removed from screen after rock despawn, kill enemy.
			if (deathWait.getElapsedTime().asSeconds() > 5.0f)
			{
				game->createScore(getPosition(), "rock");
				setActive(false);
			}
		}
	}
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
	// If ever close to player during current flight, move to former position.
	// Otherwise move towards the general position of the player.
	static const int floatDistance = 64;
	sf::Vector2f playerPos = (floatTarget.x != -1)
		? floatTarget : game->getDigDugPointer()->getPosition();
	sf::Vector2f currentPos = getPosition();

	// Checks if the enemy is near the player and sets the target to the former playerPos.
	if (abs(currentPos.x - playerPos.x) < floatDistance 
		&& abs(currentPos.y - playerPos.y) < floatDistance)
	{
		float topPlayerPosX = static_cast<int>(playerPos.x) 
			- (static_cast<int>(playerPos.x) % 16) + .05f;
		float topPlayerPosY = static_cast<int>(playerPos.y)
			- (static_cast<int>(playerPos.y) % 16) + .05f;
		if (floatTarget.x == -1)
			floatTarget = sf::Vector2f(topPlayerPosX, topPlayerPosY);
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

	// If still floating move towards player ignoring collisions,
	// otherwise move towards player regarding collision.
	if (canFloat)
	{
		int moveDir = -1;
		float nearX = 0; // Checks how close enemy is to player.
		float nearY = 0;

		// If not already at position, change nearX and nearY.
		if (currentPos.x != playerPos.x)
			nearX = abs(currentPos.x - playerPos.x);
		if (currentPos.y != playerPos.y)
			nearY = abs(currentPos.y - playerPos.y);

		// If float target is set, if near enough to player
		// set position as target position.
		if (floatTarget.x != -1)
		{
			if (nearX != 0 && nearX <= getSpeed())
				setPosition(sf::Vector2f(playerPos.x, getPosition().y));
			if (nearY != 0 && nearY <= getSpeed())
				setPosition(sf::Vector2f(getPosition().x, playerPos.y));
		}

		// If not colliding at all and at any time near the player, stop floating.
		if (getPosition() == playerPos)
		{
			floatTarget = sf::Vector2f(-1, -1);
			canFloat = false;
		}

		// Moves towards player in a diagonal pattern
		// depending on if the x or y is closer.
		if (!canFloat)
			moveDir = moveTowardPlayer();
		else if (nearY != 0 && nearY >= nearX)
		{
			if (getPosition().y >= playerPos.y)
				moveDir = up;
			else if (getPosition().y < playerPos.y)
				moveDir = down;
		}
		else if (nearX != 0 && nearX > nearY)
		{
			if (getPosition().x >= playerPos.x)
				moveDir = left;
			else if (getPosition().x < playerPos.x)
				moveDir = right;
		}

		return moveDir;
	}
	else
	{
		floatTarget = sf::Vector2f(-1, -1);
		return moveTowardPlayer();
	}
}


// Checks surrounding sand if the enemy can move through it.
// Pos is the position of the enemy while choices is changed for
// each direction of collision.
void Enemy::checkSurroundingSand(sf::Vector2f pos, bool choices[4])
{
	// Finds the array position based on the passed position.
	int arrXPos = static_cast<int>(pos.x) / 16;
	int arrYPos = (static_cast<int>(pos.y) - 32) / 16;
	// Finds the amount the x and y position is off x and y axis.
	float offXPos = (static_cast<int>(pos.x) % 16)
		+ (pos.x - static_cast<int>(pos.x));
	float offYPos = (static_cast<int>(pos.y) % 16)
		+ (pos.y - static_cast<int>(pos.y));

	for (int i = 0; i < 4; i++)
		choices[i] = false;

	// If near the side of each side, see if there is sand in the way.
	choices[0] = getSandCollision(arrXPos, arrYPos, 0);
	choices[1] = getSandCollision(arrXPos, arrYPos, 1);
	choices[2] = getSandCollision(arrXPos, arrYPos, 2);
	choices[3] = getSandCollision(arrXPos, arrYPos, 3);
}


// Checks if the inputted array position can be moved to
// from the direction passed.
bool Enemy::getSandCollision(int xPos, int yPos, int direction)
{
	// Exit of level, meaning no movement.
	if (xPos == -1)
	{
		return true;
	}

	// Top layer of ground where enemy can escape
	// Can move left, right and down, but not up.
	if (yPos == -1)
	{
		if (direction == up)
			return true;
		else if (direction == right && xPos >= 11)
			return true;
		else if (direction == right)
			return false;
		else if (direction == left)
			return false;
		else if (direction == down)
			return !game->getSandPointer(xPos
				+ ((yPos + 1) * 12))->getMove(down);
	}

	// If at the top layer of sand allow movement if possible
	// otherwise prevent movement.
	if (direction == up && yPos <= 0)
	{
		if (yPos == 0)
			return !game->getSandPointer(xPos + (yPos * 12))->getMove(down);
		else
			return true;
	}
	// Prevents movement if the enemy tries to go out of bounds.
	else if (direction == down && yPos >= 11)
		return true;
	else if (direction == left && xPos <= 0)
		return true;
	else if (direction == right && xPos >= 11)
		return true;

	// For each direction, if the current index of sand has no sand 
	// in direction and the next sand has no sand blocking the
	// chosen direction collision is false.
	Sand* const orgSandPtr = game->getSandPointer(xPos + (yPos * 12));
	if (direction == up)
	{
		Sand* const nextSandPtr = game->getSandPointer(xPos + ((yPos - 1) * 12));
		return (!orgSandPtr->getMove(down) && !nextSandPtr->getMove(up));
	}
	else if (direction == down)
	{
		Sand* const nextSandPtr = game->getSandPointer(xPos + ((yPos + 1) * 12));
		return (!orgSandPtr->getMove(up) && !nextSandPtr->getMove(down));
	}
	else if (direction == left)
	{
		Sand* const nextSandPtr = game->getSandPointer((xPos - 1) + (yPos * 12));
		return (!orgSandPtr->getMove(right) && !nextSandPtr->getMove(left));
	}
	else if (direction == right)
	{
		Sand* const nextSandPtr = game->getSandPointer((xPos + 1) + (yPos * 12));
		return (!orgSandPtr->getMove(left) && !nextSandPtr->getMove(right));
	}

	std::cerr << "Incorrect sand collision direction!\n";
	return true;
}


// Finds a path towards the given target using recursion, saving the best path.
// CurrentPos is the position in the call 'loop' while lastPath is the last
// direction, and currentPath is the path in the current loop.
void Enemy::findTarget(sf::Vector2f target, sf::Vector2f currentPos,
	int lastPath, std::vector<int> currentPath)
{
	static const int maxPathLength = 12; // Game lags if too high,
	// but movement can still occur without finding a direct path.

	// If at the target and no path is found or the path is shorter than
	// any previous paths, save the path to path.
	if (currentPos.x < target.x + 16 && currentPos.x >= target.x &&
		currentPos.y < target.y + 16 && currentPos.y >= target.y)
	{
		if (currentPath.size() <= path.size() || pathEndFound == false)
		{
			pathEndFound = true;
			path = currentPath;
		}
	}

	// Returns if the current path size becomes too large.
	if (currentPath.size() > maxPathLength)
		return;

	bool collisions[4];
	// Checks the surrounding sand for collisions.
	checkSurroundingSand(currentPos, collisions);
	// For each direction, if possible, moves and finds a path
	// towards the player saving the path through a
	// push_back before the recursive call and a pop_back after.
	for (int i = 0; i < 4; i++)
	{
		if (collisions[i] == false)
		{
			if (i == up && lastPath != down)
			{
				currentPath.push_back(i);
				findTarget(target, sf::Vector2f(currentPos.x,
					currentPos.y - 16), up, currentPath);
				currentPath.pop_back();
			}
			if (i == down && lastPath != up)
			{
				currentPath.push_back(i);
				findTarget(target, sf::Vector2f(currentPos.x,
					currentPos.y + 16), down, currentPath);
				currentPath.pop_back();
			}
			if (i == left && lastPath != right)
			{
				currentPath.push_back(i);
				findTarget(target, sf::Vector2f(currentPos.x - 16,
					currentPos.y), left, currentPath);
				currentPath.pop_back();
			}
			if (i == right && lastPath != left)
			{
				currentPath.push_back(i);
				findTarget(target, sf::Vector2f(currentPos.x + 16,
					currentPos.y), right, currentPath);
				currentPath.pop_back();
			}
		}
	}
}

// Resets player to default values.
void Enemy::reset(sf::Vector2f pos)
{
	GameObject::reset(pos);

	path.clear();
	currentCollides = 0;
	currentPath = false;
	pathEndFound = false;
	deathType = "none";
	pumpClock.restart();
	escapeTimer.restart();
	canFloat = false;
	floatTarget = sf::Vector2f(-1, -1);
	lastSquare = sf::Vector2f(-1, -1);
	currentPump = 0;
	// Speeds up every 12 rounds.
	speed = .25f + (static_cast<int>(game->getLevel() / 12) * .025f);
	initialPosition = pos;

	spritesheet.setSize(sf::Vector2i(16, 16), sf::Vector2i(0, 0), 0);

	for (int i = 0; i < 4; i++)
		sandCollided[i] = false;
}