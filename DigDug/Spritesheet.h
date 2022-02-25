#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Spritesheet : public sf::Sprite
{
public:
	Spritesheet();
	void setupSprite(std::string, sf::Vector2i, sf::Vector2i);
	virtual bool loadSprite(int currentSprite);

private:
	sf::Texture texture;
	sf::Vector2i totalSize;
	sf::Vector2i spriteSize;
	std::string textureFile;
};

