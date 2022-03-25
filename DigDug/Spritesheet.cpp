#include "Spritesheet.h"


Spritesheet::Spritesheet()
{
	texture = sf::Texture();
	totalSize = sf::Vector2i(0, 0);
	spriteSize = sf::Vector2i(0, 0);
	textureFile = "unset";
	setTexture(texture);
	xTile = 0;
	yTile = 0;
}


// Sets up the sprite based on the passed file name,
// total size of sprite and the size for each frame.
void Spritesheet::setupSprite(std::string textureFile,
	sf::Vector2i totalSize, sf::Vector2i spriteSize)
{
	// If cannot load texture, output and error.
	if (!texture.loadFromFile(textureFile))
		std::cerr << "Warning: Could not load " << textureFile << "!\n";

	this->totalSize = totalSize;
	this->spriteSize = spriteSize;
	this->textureFile = textureFile;

	// Loads initial sprite.
	loadSprite(0);
}


// Loads the specified index of a spritesheet.
bool Spritesheet::loadSprite(int currentSprite)
{
	xTile = 0;
	yTile = 0;

	if (spriteSize.x != 0)
	{
		// Finds the xTile of the sprite, wrapping to 0 if necessary.
		xTile = ((currentSprite * spriteSize.x) % totalSize.x) / spriteSize.x;
		// Finds the yTile of the sprite, going down for each wrap of the x.
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
	setTextureRect(sf::IntRect(xTile * spriteSize.x + startPos.x,
		yTile * spriteSize.y + startPos.y, 
		spriteSize.x, spriteSize.y));

	return true;
}


// Sets/Changes the size, starting position,
// and starting index of the spritesheet.
void Spritesheet::setSize(sf::Vector2i size, sf::Vector2i start, int index)
{
	// Makes sure that the new size isn't larger than the tilesheet.
	if (size.x <= totalSize.x && size.y <= totalSize.y)
	{
		startPos = start;
		spriteSize = size;
		loadSprite(index);
	}
}


// Returns current position of the spritesheet.
sf::Vector2f Spritesheet::getIndex()
{
	return sf::Vector2f(xTile, yTile);
}
