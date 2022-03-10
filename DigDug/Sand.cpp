#include "Sand.h"
#include "Game.h"


Sand::Sand() : Sand(nullptr, nullptr)
{ 
	foreground.setupSprite("Images/foregroundSpritesheet.png",
		sf::Vector2i(240, 16), sf::Vector2i(16, 16));
	background2.setupSprite("Images/backgroundSpritesheet.png",
		sf::Vector2i(192, 16), sf::Vector2i(16, 16));

	// Sets correct sprites to foreground and background.
	foreground.loadSprite(0);
	background2.loadSprite(0);

	// Allow for correct positioning on the mask/texture.
	topActive = true;
	reset(1, true);
}


Sand::~Sand()
{
	// Deconstructor
}


Sand::Sand(sf::RenderWindow* win, Game* game)
	: window(win), game(game)
{
	foreground.setupSprite("Images/foregroundSpritesheet.png",
		sf::Vector2i(240, 16), sf::Vector2i(16, 16));
	background2.setupSprite("Images/backgroundSpritesheet.png",
		sf::Vector2i(192, 16), sf::Vector2i(16, 16));

	// Sets correct sprites to foreground and background.
	foreground.loadSprite(0);
	background2.loadSprite(0);

	// Allow for correct positioning on the mask/texture.
	topActive = true;
	reset(1, true);
}


// Changes the top background mask to the correct size based on current mask and inputted position/direction.
// Moves this mask into the correct position and chooses the correct foreground sprite for the "path".
void Sand::changeSand(sf::Vector2f playerPos, int dir)
{
	// Finds x and y change from the player to the top left of the
	// sand object and adds 16 to prevent negatives -> 32.
	int xChange = playerPos.x - foreground.getPosition().x + 16;
	int yChange = playerPos.y - foreground.getPosition().y + 16;

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
			bottomMask = (bottomMask > 32 - yChange) ? bottomMask : 32 - yChange; // Check if this is necessary...
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
	background2.setTextureRect(sf::IntRect(
		background2.getIndex().x * 16 + leftMask,
		background2.getIndex().y * 16 + topMask,
		width,
		height
	));

	// Moves the sprite so the increase doesn't always come from the bottom of the mask when
	// the topMask is changed instead of the bottomMask.
	background2.setPosition(
		sf::Vector2f(foreground.getPosition().x + leftMask, foreground.getPosition().y + topMask)
	);
}


void Sand::reset(int round, bool full)
{
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

		int lowerRound = round % 12;
		int sandPos = (getPosition().y - 32) / 16 / 3;
		int sandChoice = 0;

		if (lowerRound >= 1 || lowerRound < 5)
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
		else if (lowerRound >= 5 || lowerRound < 9)
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

		background2.loadSprite(sandChoice);

		if (!getTopActive())
		{
			bool sandCollided[4];

			for (int i = 0; i < 4; i++)
				sandCollided[i] = false;

			sf::FloatRect mainCollider = getBackCollider();
			mainCollider.left += .125f;
			mainCollider.width -= .25f;
			mainCollider.top += .125f;
			mainCollider.height -= .25f;

			sf::FloatRect topCollider = mainCollider;
			topCollider.top -= .25f;

			sf::FloatRect bottomCollider = mainCollider;
			bottomCollider.height += .25f;

			sf::FloatRect leftCollider = mainCollider;
			leftCollider.left -= .25f;

			sf::FloatRect rightCollider = mainCollider;
			rightCollider.width += .25f;

			for (int i = 0; i < game->getArrLength(Game::Object::sandSand); i++)
			{
				if (game->checkCollision(topCollider, Game::Object::sandSand, i))
					sandCollided[0] = true;
				if (game->checkCollision(bottomCollider, Game::Object::sandSand, i))
					sandCollided[1] = true;
				if (game->checkCollision(leftCollider, Game::Object::sandSand, i))
					sandCollided[2] = true;
				if (game->checkCollision(rightCollider, Game::Object::sandSand, i))
					sandCollided[3] = true;
			}

			if (sandCollided[0])
				bottomMask = 16;
			if (sandCollided[1])
				topMask = 16;
			if (sandCollided[2])
				rightMask = 16;
			if (sandCollided[3])
				leftMask = 16;

			upMove = !sandCollided[0];
			downMove = !sandCollided[1];
			leftMove = !sandCollided[2];
			rightMove = !sandCollided[3];

			background2.setTextureRect(sf::IntRect(0, 0, 0, 0));

			setSprite();
		}
	}
}


void Sand::setSprite()
{
	if (upMove && downMove && rightMove && leftMove)
		foreground.loadSprite(14);
	else if (upMove && downMove && leftMove)
		foreground.loadSprite(12);
	else if (upMove && downMove && rightMove)
		foreground.loadSprite(13);
	else if (downMove && leftMove && rightMove)
		foreground.loadSprite(10);
	else if (upMove && leftMove && rightMove)
		foreground.loadSprite(11);
	else if (upMove && rightMove)
		foreground.loadSprite(8);
	else if (upMove && leftMove)
		foreground.loadSprite(9);
	else if (downMove && rightMove)
		foreground.loadSprite(6);
	else if (downMove && leftMove)
		foreground.loadSprite(7);
	else if (leftMove && rightMove)
		foreground.loadSprite(5);
	else if (upMove && downMove)
		foreground.loadSprite(4);
	else if (rightMove)
		foreground.loadSprite(2);
	else if (leftMove)
		foreground.loadSprite(3);
	else if (downMove)
		foreground.loadSprite(0);
	else
		foreground.loadSprite(1);
}


// Returns the collider/FloatRect of the object to check
// collisions with player.
sf::FloatRect& Sand::getBackCollider()
{
	boundingBox = foreground.getGlobalBounds();

	return boundingBox;
}


// Returns the collider/FloatRect of the object to check
// collisions with other objects.
sf::FloatRect& Sand::getForeCollider()
{
	boundingBox = background2.getGlobalBounds();

	return boundingBox;
}


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


void Sand::drawObject()
{
	if (getBackActive())
	{
		window->draw(foreground);
		window->draw(background2);
	}
}

void Sand::setActive(const bool& active)
{
	backActive = active;
}


// If false, check sides, if true, check nothing.
void Sand::preReset(const bool& full, sf::Vector2f pos)
{
	foreground.setPosition(pos);
	background2.setPosition(pos);
	setActive(true);
	topActive = full;
}


bool Sand::getBackActive()
{
	return backActive;
}


bool Sand::getTopActive()
{
	return topActive;
}


sf::Vector2f Sand::getPosition()
{
	return foreground.getPosition();
}