#include "Sand.h"


Sand::Sand() : Sand(nullptr, nullptr, nullptr, 0, 0, sf::Vector2f(0, 0), nullptr)
{ }


Sand::~Sand()
{
	// Deconstructor
}


Sand::Sand(Spritesheet* fore, Spritesheet* back1, Spritesheet* back2, int initialFore, int initialBack, sf::Vector2f pos, sf::RenderWindow* win)
	: foreground(fore), background(back1), background2(back2), window(win)
{
	// Moves each spritesheet to initial location.
	foreground->setPosition(pos);
	background->setPosition(pos);
	background2->setPosition(pos);

	// Sets correct sprites to foreground and background.
	foreground->loadSprite(initialFore);
	background->loadSprite(initialBack);
	background2->loadSprite(initialBack);

	// Allow for correct positioning on the mask/texture.
	initialBackground = initialBack;
	initialForeground = initialFore;
}


// Changes the top background mask to the correct size based on current mask and inputted position/direction.
// Moves this mask into the correct position and chooses the correct foreground sprite for the "path".
void Sand::changeSand(sf::Vector2f playerPos, int dir)
{
	// Finds x and y change from the player to the top left of the
	// sand object and adds 16 to prevent negatives -> 32.
	int xChange = playerPos.x - background->getPosition().x + 16;
	int yChange = playerPos.y - background->getPosition().y + 16;

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

	// Finds out which sprite to load based on the current corresponding masking.
	// Should be optimized, probably >.<
	if (topMask > 0 && bottomMask > 0 && leftMask > 0 && rightMask > 0)
		foreground->loadSprite(15);
	else if (topMask > 0 && bottomMask > 0 && leftMask > 0)
		foreground->loadSprite(13);
	else if (topMask > 0 && bottomMask > 0 && rightMask > 0)
		foreground->loadSprite(14);
	else if (bottomMask > 0 && leftMask > 0 && rightMask > 0)
		foreground->loadSprite(11);
	else if (topMask > 0 && leftMask > 0 && rightMask > 0)
		foreground->loadSprite(12);
	else if (topMask > 0 && rightMask > 0)
		foreground->loadSprite(9);
	else if (topMask > 0 && leftMask > 0)
		foreground->loadSprite(10);
	else if (bottomMask > 0 && rightMask > 0)
		foreground->loadSprite(7);
	else if (bottomMask > 0 && leftMask > 0)
		foreground->loadSprite(8);
	else if (leftMask > 0 || rightMask > 0)
		foreground->loadSprite(5);
	else if (topMask > 0 || bottomMask > 0)
		foreground->loadSprite(4);

	// Make sure that the current height and width is not greater than 16.
	int height = (topMask + bottomMask > 16) ? 0 : 16 - topMask - bottomMask;
	int width = (leftMask + rightMask > 16) ? 0 : 16 - leftMask - rightMask;

	// Sets the top and the left of the mask to the current sprite plus the mask.
	// Sets the width and height to the total masking or 16.
	background2->setTextureRect(sf::IntRect(
		initialBackground * 16 + leftMask,
		initialBackground * 16 + topMask,
		width,
		height
	));

	// Moves the sprite so the increase doesn't always come from the bottom of the mask when
	// the topMask is changed instead of the bottomMask.
	background2->setPosition(
		sf::Vector2f(background->getPosition().x + leftMask, background->getPosition().y + topMask)
	);
}


void Sand::reset(int spriteIndex, sf::Vector2f pos, int initialFore, int initialBack)
{
	// Resets the values of sand to load for the next level. 
	// Sprite index is for color changes.
	
	background2->setPosition(pos);
	background2->setTextureRect(sf::IntRect(initialFore, initialBack, 16, 16));
	background2->loadSprite(spriteIndex);
	background->loadSprite(spriteIndex);
}


// Returns the collider/FloatRect of the object to check
// collisions with other objects.
sf::FloatRect& Sand::getCollider()
{
	boundingBox = background->getGlobalBounds();

	return boundingBox;
}


void Sand::drawObject()
{
	window->draw(*background);
	window->draw(*foreground);
	window->draw(*background2);
}
