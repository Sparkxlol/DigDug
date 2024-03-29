#include "Sand.h"
#include "Game.h"


Sand::Sand() : Sand(nullptr, nullptr)
{ 
	background.setupSprite("Images/foregroundSpritesheet.png",
		sf::Vector2i(240, 16), sf::Vector2i(16, 16));
	foreground.setupSprite("Images/backgroundSpritesheet.png",
		sf::Vector2i(192, 16), sf::Vector2i(16, 16));

	// Sets correct sprites to foreground and background.
	background.loadSprite(0);
	foreground.loadSprite(0);

	// Allow for correct positioning on the mask/texture.
	topActive = true;
	backActive = true;
	topMask = 0;
	bottomMask = 0;
	leftMask = 0;
	rightMask = 0;
	upMove = false;
	downMove = false;
	leftMove = false;
	rightMove = false;
}


Sand::~Sand()
{
	// Deconstructor
}


Sand::Sand(sf::RenderWindow* win, Game* game)
	: window(win), game(game)
{
	background.setupSprite("Images/foregroundSpritesheet.png",
		sf::Vector2i(240, 16), sf::Vector2i(16, 16));
	foreground.setupSprite("Images/backgroundSpritesheet.png",
		sf::Vector2i(192, 16), sf::Vector2i(16, 16));

	// Sets correct sprites to foreground and background.
	background.loadSprite(0);
	foreground.loadSprite(0);

	// Allow for correct positioning on the mask/texture.
	topActive = true;
	backActive = true;
	topMask = 0;
	bottomMask = 0;
	leftMask = 0;
	rightMask = 0;
	upMove = false;
	downMove = false;
	leftMove = false;
	rightMove = false;
}


/* Changes the top background mask to the correct size based 
* on current mask and inputted position/direction.
* Moves this mask into the correct position and chooses the
* correct foreground sprite for the "path".
* Returns true if the sand was changed, false if not. */
bool Sand::changeSand(sf::Vector2f playerPos, int dir)
{
	// Finds x and y change from the player to the top left of the
	// sand object and adds 16 to prevent negatives -> 32.
	int xChange = playerPos.x - background.getPosition().x + 16;
	int yChange = playerPos.y - background.getPosition().y + 16;

	// Finds out which sprite to load based on the current corresponding masking.
	// Should be optimized, probably >.<

	if (yChange < 15.5)
		upMove = true;
	else if (yChange > 16.5)
		downMove = true;

	if (xChange < 15.5)
		leftMove = true;
	else if (xChange > 16.5)
		rightMove = true;

	setSprite();

	switch (dir)
	{
	case 0:
		// If not moving up in the top part and not fewer than current
		// change, make bottomMask either current size or new change from bottom.
		if (yChange > topMask && 32 - yChange > bottomMask)
			bottomMask = (bottomMask > 32 - yChange) ? bottomMask : 32 - yChange; 
		break;
	case 1:
		// If not moving down at the bottom and not fewer than current
		// change, make topMask either the current yChange or the current topMask.
		if (yChange < 32 - bottomMask && yChange > topMask)
			topMask = (topMask > yChange) ? topMask : yChange;
		break;
	case 2:
		// Works same as bottomMask case, just with rightMask instead.
		if (xChange > leftMask && 32 - xChange > rightMask)
			rightMask = (rightMask > 32 - xChange) ? rightMask : 32 - xChange;
		break;
	case 3:
		// Works same as topMask case, just with leftMask instead.
		if (xChange < 32 - rightMask && xChange > leftMask)
			leftMask = (leftMask > xChange) ? leftMask : xChange;
		break;
	}

	// Make sure that the current height and width is not greater than 16.
	int height = (topMask + bottomMask > 16) ? 0 : 16 - topMask - bottomMask;
	int width = (leftMask + rightMask > 16) ? 0 : 16 - leftMask - rightMask;

	// Sets the top and the left of the mask to the current sprite plus the mask.
	// Sets the width and height to the total masking or 16.
	foreground.setTextureRect(sf::IntRect(
		foreground.getIndex().x * 16 + leftMask,
		foreground.getIndex().y * 16 + topMask,
		width,
		height
	));

	// Moves the sprite so the increase doesn't always come from the bottom of the mask when
	// the topMask is changed instead of the bottomMask.
	foreground.setPosition(
		sf::Vector2f(background.getPosition().x + leftMask,
			background.getPosition().y + topMask)
	);

	// Returns if sand is changed from start or not.
	return !(height == 0 || width == 0);
}


// Resets the sand based on the corresponding round.
void Sand::reset(int round, bool full, bool sandLoc[12][12])
{
	// If a full reset (meaning different level from last)
	// Change the path and masking based on surrounding sand.
	if (full)
	{
		topMask = 0; 
		bottomMask = 0;
		leftMask = 0; 
		rightMask = 0;
		upMove = false;
		downMove = false;
		leftMove = false;
		rightMove = false;

		int lowerRound = round % 12; // Resets sand color every 12 rounds.
		// Finds the level of the sand, 4 levels (color) in total.
		int sandPos = (getPosition().y - 32) / 16 / 3;
		int sandChoice = 0;

		// Changes background sprite based on round and depth.
		if (lowerRound >= 0 && lowerRound < 5)
		{
			if (sandPos == 0)
				sandChoice = 0;
			else if (sandPos == 1)
				sandChoice = 1;
			else if (sandPos == 2)
				sandChoice = 2;
			else
				sandChoice = 3;
		}
		else if (lowerRound >= 5 && lowerRound < 9)
		{
			if (sandPos == 0)
				sandChoice = 4;
			else if (sandPos == 1)
				sandChoice = 5;
			else if (sandPos == 2)
				sandChoice = 6;
			else
				sandChoice = 7;
		}
		else
		{
			if (sandPos == 0)
				sandChoice = 8;
			else if (sandPos == 1)
				sandChoice = 9;
			else if (sandPos == 2)
				sandChoice = 10;
			else
				sandChoice = 11;
		}

		foreground.loadSprite(sandChoice);

		int xPos = getPosition().x / 16;
		int yPos = (getPosition().y - 32) / 16;
			
		// If sandLoc is false, open path, otherwise full path.
		if (sandLoc[yPos][xPos] == false)
		{
				// If collided on any side, the mask is set to allow digging.
			if (xPos <= 0 || sandLoc[yPos][xPos - 1] == false)
			{
				leftMove = true;
				leftMask = 16;
			}
			if (xPos >= 11 || sandLoc[yPos][xPos + 1] == false)
			{
				rightMove = true;
				rightMask = 16;
			}
			if (yPos <= 0 || sandLoc[yPos - 1][xPos] == false)
			{
				upMove = true;
				topMask = 16;
			}
			if (yPos >= 11 || sandLoc[yPos + 1][xPos] == false)
			{
				downMove = true;
				bottomMask = 16;
			}

			foreground.setTextureRect(sf::IntRect(0, 0, 0, 0));

			setSprite(); // Changes the sprite based on these collisions.
		}
		// Row leading to dig dug has to not be checked by surrounding sand.
		else if (sandLoc[yPos][xPos] == true)
		{
			if (yPos < 5 && xPos == 5)
			{
				upMove = true;
				topMask = 16;
				downMove = true;
				bottomMask = 16;
				foreground.setTextureRect(sf::IntRect(0, 0, 0, 0));
				setSprite();
			}
			else if (yPos == 5 && xPos == 5)
			{
				upMove = true;
				topMask = 16;
				leftMove = true;
				leftMask = 16;
				rightMove = true;
				rightMask = 16;
				foreground.setTextureRect(sf::IntRect(0, 0, 0, 0));
				setSprite();
			}
			else if (yPos == 5 && xPos == 4)
			{
				rightMove = true;
				rightMask = 16;
				foreground.setTextureRect(sf::IntRect(0, 0, 0, 0));
				setSprite();
			}
			else if (yPos == 5 && xPos == 6)
			{
				leftMove = true;
				leftMask = 16;
				foreground.setTextureRect(sf::IntRect(0, 0, 0, 0));
				setSprite();
			}
		}
	}
	else
	{
		foreground.setPosition(
			sf::Vector2f(background.getPosition().x + leftMask,
				background.getPosition().y + topMask)
		);
	}
}


// Changes appearance of path based on which sides are "open"
// or which sides can be moved towards.
void Sand::setSprite()
{
	if (upMove && downMove && rightMove && leftMove)
		background.loadSprite(14);
	else if (upMove && downMove && leftMove)
		background.loadSprite(12);
	else if (upMove && downMove && rightMove)
		background.loadSprite(13);
	else if (downMove && leftMove && rightMove)
		background.loadSprite(10);
	else if (upMove && leftMove && rightMove)
		background.loadSprite(11);
	else if (upMove && rightMove)
		background.loadSprite(8);
	else if (upMove && leftMove)
		background.loadSprite(9);
	else if (downMove && rightMove)
		background.loadSprite(6);
	else if (downMove && leftMove)
		background.loadSprite(7);
	else if (leftMove && rightMove)
		background.loadSprite(5);
	else if (upMove && downMove)
		background.loadSprite(4);
	else if (rightMove)
		background.loadSprite(2);
	else if (leftMove)
		background.loadSprite(3);
	else if (downMove)
		background.loadSprite(0);
	else
		background.loadSprite(1);
}


// Returns the collider/FloatRect of the object to check
// collisions with player.
sf::FloatRect& Sand::getBackCollider()
{
	boundingBox = background.getGlobalBounds();

	return boundingBox;
}


// Returns the collider/FloatRect of the object to check
// collisions with other objects.
sf::FloatRect& Sand::getForeCollider()
{
	boundingBox = foreground.getGlobalBounds();

	return boundingBox;
}


// Checks each side of the sand for collisions on the passed collider.
// Uses a point on the end of the sand to check if the collider contains them.
bool Sand::checkTopCollider(const sf::FloatRect& otherColl)
{
	bool collided = false;

	if (!upMove &&
		otherColl.contains(sf::Vector2f(getPosition().x + 8, getPosition().y)))
		collided = true;
	if (!downMove &&
		otherColl.contains(sf::Vector2f(getPosition().x + 8, getPosition().y + 16)))
		collided = true;
	if (!leftMove &&
		otherColl.contains(sf::Vector2f(getPosition().x, getPosition().y + 8)))
		collided = true;
	if (!rightMove &&
		otherColl.contains(sf::Vector2f(getPosition().x + 16, getPosition().y + 8)))
		collided = true;

	return collided;
}


// Draws both the path and the sand top.
void Sand::drawObject()
{
	if (getBackActive())
	{
		window->draw(background);
		window->draw(foreground);
	}
}

// Sets the path activity (used for drawing and collisions).
void Sand::setActive(const bool& active)
{
	backActive = active;
}


// If false, check sides, if true, check nothing.
void Sand::preReset(sf::Vector2f pos)
{
	background.setPosition(pos);
	foreground.setPosition(pos);
	setActive(true);
}


// Returns the path activity (used for drawing/collisions with enemies).
bool Sand::getBackActive()
{
	return backActive;
}


// Returns the top sand activity (used for digDug collisions).
bool Sand::getTopActive()
{
	return topActive;
}


bool Sand::getMove(int direction)
{
	if (direction == 1)
		return upMove;
	else if (direction == 0)
		return downMove;
	else if (direction == 3)
		return leftMove;
	else
		return rightMove;
}


// Returns the position of the sand.
sf::Vector2f Sand::getPosition()
{
	return background.getPosition();
}