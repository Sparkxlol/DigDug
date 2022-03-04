#include "Spritesheet.h"


Spritesheet::Spritesheet()
{
	texture = sf::Texture();
	totalSize = sf::Vector2i(0, 0);
	spriteSize = sf::Vector2i(0, 0);
	textureFile = "unset";
	setTexture(texture);
}


void Spritesheet::setupSprite(std::string textureFile, sf::Vector2i totalSize, sf::Vector2i spriteSize)
{
	if (!texture.loadFromFile(textureFile))
		std::cerr << "Warning: Could not load " << textureFile << "!\n";

	this->totalSize = totalSize;
	this->spriteSize = spriteSize;
	this->textureFile = textureFile;

	loadSprite(0);
}


bool Spritesheet::loadSprite(int currentSprite)
{
	int xTile = 0;
	int yTile = 0;

	if (spriteSize.x != 0)
	{
		xTile = ((currentSprite * spriteSize.x) % totalSize.x) / spriteSize.x;
		yTile = static_cast<float>(currentSprite * spriteSize.x / totalSize.x);
	}

	// Catches out of bound attempts.
	if (xTile >= totalSize.x || yTile >= totalSize.y)
	{
		std::cerr << "WARNING: Could not load tile at " 
			<< currentSprite << " in " << textureFile << "!\n";
		return false;
	}


	// Sets the sprite to the correponding tile based on currentSprite.
	// IntRect has xPos, yPos, width, height.
	setTextureRect(sf::IntRect(xTile * spriteSize.x + startPos.x, yTile * spriteSize.y + startPos.y, 
		spriteSize.x, spriteSize.y));

	return true;
}


void Spritesheet::setSize(sf::Vector2i size, sf::Vector2i start, int index)
{
	if (size.x <= totalSize.x && size.y <= totalSize.y)
	{
		startPos = start;
		spriteSize = size;
		loadSprite(index);
	}
}