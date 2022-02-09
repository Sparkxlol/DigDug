#include "Spritesheet.h"


Spritesheet::Spritesheet(std::string textureFile, sf::Vector2i totalSize, 
	sf::Vector2i spriteSize)
{
	if (!texture.loadFromFile(textureFile))
		std::cerr << "Warning: Could not load " << textureFile << "!\n";

	setTexture(texture);

	this->totalSize = totalSize;
	this->spriteSize = spriteSize;

	loadSprite(0);
}


bool Spritesheet::loadSprite(int currentSprite)
{
	int xTile = ((static_cast<float>(currentSprite) / spriteSize.x)
		- (currentSprite / spriteSize.x)) * spriteSize.x;
	int yTile = currentSprite / spriteSize.x;

	// Catches out of bound attempts.
	if (xTile >= totalSize.x || yTile >= totalSize.y)
	{
		std::cerr << "WARNING: Couldn't load tile at " 
			<< currentSprite << " in " << textureFile << "!";
		return false;
	}

	std::cout << xTile << " " << yTile << std::endl;

	// Sets the sprite to the correponding tile based on currentSprite.
	// IntRect has xPos, yPos, width, height.
	setTextureRect(sf::IntRect(xTile * spriteSize.x, yTile * spriteSize.y, 
		spriteSize.x, spriteSize.y));

	return true;
}