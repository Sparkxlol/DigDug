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
	foreground->setPosition(pos);
	background->setPosition(pos);
	background2->setPosition(pos);

	// Sets correct sprites to foreground and background.
	// Sets x and y mask / direction to corresponding foreground.
	foreground->loadSprite(15);
	background->loadSprite(initialBack);
	background2->loadSprite(initialBack);

	initialBackground = initialBack;
	initialForeground = initialFore;
}


void Sand::changeSand(sf::Vector2f playerPos, int dir)
{
	// Change x and y mask to correct values based on inputted direction and mask
	// Use mask values to edit spritesheet texture rect.

	int xChange = playerPos.x - background->getPosition().x + 16;
	int yChange = playerPos.y - background->getPosition().y + 16;

	switch (dir)
	{
	case 0:
		// If moving up and pos is greater than topMask and less than bottomMask
		// increase bottomMask
		if (yChange > topMask && 32 - yChange > bottomMask)
			bottomMask = (bottomMask > 32 - yChange) ? bottomMask : 32 - yChange; // Check if this is necessary...
		break;
	case 1:
		// If moving down and pos is less than bottomMask and greater than topMask
		// increase topMask
		if (yChange < 32 - bottomMask && yChange > topMask)
			topMask = (topMask > yChange) ? topMask : yChange;
		break;
	}

	//std::cout << bottomMask << " " << topMask << std::endl;

	/*
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
		foreground->loadSprite(6);
	else if (topMask > 0 || bottomMask > 0)
		foreground->loadSprite(5);
	*/

	int height = (topMask + bottomMask > 16) ? 0 : 16 - topMask - bottomMask;

	background2->setTextureRect(sf::IntRect(
		initialBackground * 16,
		initialBackground * 16 + topMask,
		16,
		height
	));

	background2->setPosition(
		sf::Vector2f(background->getPosition().x + leftMask, background->getPosition().y + topMask)
	);
}


void Sand::reset(int spriteIndex, int initialFore, int initialBack)
{
	// Resets the values of sand to load for the next level. 
	// Sprite index is for color changes.
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