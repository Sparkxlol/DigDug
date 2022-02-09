#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Spritesheet : public sf::Sprite
{
public:
	Spritesheet(std::string textureFile, sf::Vector2i totalSize,
		sf::Vector2i spriteSize);
	virtual bool loadSprite(int currentSprite);

private:
	sf::Texture texture;
	sf::Vector2i totalSize;
	sf::Vector2i spriteSize;
	std::string textureFile;
};

